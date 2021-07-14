
#include <lx16a-servo.h>


#include <chrono>
#include <serialib.h>

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// write a command with the provided parameters
// returns true if the command was written without conflict onto the bus
bool LX16ABus::write_no_retry(uint8_t cmd, const uint8_t *params, int param_cnt,
		uint8_t MYID) {
	if (param_cnt < 0 || param_cnt > 4)
		return false;

	// prepare packet in a buffer
	int buflen = 6 + param_cnt;
  
	uint8_t buf[buflen];
    uint8_t ret[buflen];
	buf[0] = 0x55;
	buf[1] = 0x55;
	buf[2] = MYID;
	buf[3] = buflen - 3;
	buf[4] = cmd;
	for (int i = 0; i < param_cnt; i++)
		buf[5 + i] = params[i];
	uint8_t cksum = 0;
	for (int i = 2; i < buflen - 1; i++)
		cksum += buf[i];
	buf[buflen - 1] = ~cksum;

// clear input buffer


	while (available()) {

			read();

	}

	lastCommand = cmd;
	// send command packet
	uint64_t t0 = timeSinceEpochMillisec();

	write(buf, buflen);


	// expect to read back command by virtue of single-pin loop-back
	uint64_t tout = time(buflen+4) + 4; // 2ms margin
	int got = 0;
	bool ok = true;
	if (_debug)
		printf("RCV: ");
	while ((got < buflen) && ((timeSinceEpochMillisec() - t0) < tout)) {
        if (available()) {
			ret[got] = read();
			if (ret[got] != buf[got]) {
				ok = false;
			}
			got++;
		}
	}
	if (got<buflen){
		ok = false;
	}

    if (_debug) { if (ok) printf(" OK\n"); else printf(" ERR\n"); }
	return ok;
}
bool LX16ABus::rcv(uint8_t cmd, uint8_t *params, int param_len, uint8_t MYID) {
	// read back the expected response
	uint64_t t0 = timeSinceEpochMillisec();
	uint64_t tout = time(param_len + 6) + 30; // 20ms for the servo to think
	int got = 0;
	uint8_t sum = 0;
	if (_debug)
		printf("RCV Data: ");
	int len = 7; // minimum length
    while (got < len && ((timeSinceEpochMillisec() - t0) < tout)) {
	
        
        if (available()) {
        	int ch = read();
			if (_debug)
				printf(" 0x%02x", ch);
			switch (got) {
			case 0:
			case 1:
				if (ch != 0x55) {
				if (_debug)
						printf(" ERR (hdr)\n");
					return false;
				}
				break;
			case 2:
				if (ch != MYID && MYID != 0xfe) {
					if (_debug)
						printf(" ERR (id)\n");
					return false;
				}
				break;
			case 3:
				if (ch < 3 || ch > 7) {
					if (_debug)
						printf(" ERR (len)\n");
					return false;
				}
				len = ch + 3;
				if (len > param_len + 6) {
					if (_debug)
                    	printf(
								" ERR (param_len) got %d  expected %d",len,(param_len + 6));
					return false;
				}
				break;
			case 4:
				if (ch != cmd) {
					if (_debug)
						printf(" ERR (cmd)\n");
					return false;
				}
				break;
			default:
				if (got == len - 1) {
					if ((uint8_t) ch == (uint8_t) ~sum) {
						if (_debug)
							printf(" OK\n");
						return true;
					} else {
						if (_debug)
							printf(" ERR (cksum!=%02x)\n", ~sum);
						return false;
					}
				}
				if (got - 5 > param_len) {
					if (_debug)
						printf(" ERR (cksum)\n");
					return false;
				}
				params[got - 5] = ch;
			}
			if (got > 1)
				sum += ch;
			got++;
		}
	}

	return false;
}
// read sends a command to the servo and reads back the response into the params buffer.
// returns true if everything checks out correctly.
bool LX16ABus::read_no_retry(uint8_t cmd, uint8_t *params, int param_len,
		uint8_t MYID) {
	// send the read command
	bool ok = write(cmd, NULL, 0, MYID);

	return rcv(cmd, params, param_len, MYID);
}

