/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */




#include "lx16a-servo.h"
  


int main (int argc, char** argv){

    LX16ABus servoBus;
    LX16AServo servo(&servoBus, 1);

    
    servoBus.begin();
     while (1)
     {

         printf("Pos %d \n", servo.pos_read());

        sleep(1);
     
    }
}

