# LX-16a, LX-224, LX-15d, LX-20s,... Servo Library
Simple CPP library to operate LX-16A, LX-224, LX-15D and other bus link serial servos.


This library sends simple commands to LewanSoul LX-16A, LX-224 and Hiwonder LX-15D serial bus servos using the BusLinker v2.2.

## Requirements
* cmake
* make
* gcc


## Instruction
```bash
git clone https://github.com/SpaceMaster85/lx-16a.git
cd lx-16a
cmake .
make
./example
```

## Example
The example code is in the main.cpp and reads the position of the servo with ID 1
