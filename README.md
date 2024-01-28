# MQP-arm-code

Code for arms for social robot MQP. 
All the important code so far is in the catkin_mqp folder in the src/arm_code/src folder. 
The main file is Robot.cpp, with functions for writing to the hid device using hidapi in SimpleComsDevice.cpp

## On hidapi:
Must add "hidapi/hidapi" to target_link_libraries statement in CMakeLists.txt 

### install:
- sudo apt-get install libusb-dev
- sudo apt-get install libusb-1.0-0

### used the include statements:
- include <hidapi.h>
- include <hidapi/hidapi.h>
- include <libusb.h>
- include <hidapi_libusb.h>