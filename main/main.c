// program for testing ENS160 i2c sensor with a Raspberry Pi Model 2B
// include raspberry pi i2c library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// include pigpio library
#include <pigpio.h>
#include "ens160.h"

int main() {
// initialize pigpio library
if (gpioInitialise() < 0) {
    printf("pigpio initialization failed\n");
    return 1;
}

int handle = i2cOpen(1, 0x53, 0); // Open I2C bus 1 with device address 0x53
if (handle < 0) {
    printf("Failed to open I2C bus\n");
    gpioTerminate();
    return 1;
}

// insert 200ms delay for I2C device to stabilize
time_sleep(0.2);

// Call your ENS160 functions here
// set ENS160 device to standard mode
ENS160_MODE_SET(handle, ENS160_MODE_STANDARD);

// insert 1000ms delay for I2C device to stabilize
time_sleep(1.0);

// read AQI value from ENS160 device
uint8_t aqi;
ENS160_GET_AQI(handle, &aqi);
printf("AQI: %d\n", aqi);

// read TVOC value from ENS160 device
uint16_t tvoc;
ENS160_GET_TVOC(handle, &tvoc);
printf("TVOC: %d\n", tvoc); 

uint16_t eco2;
ENS160_GET_eCO2(handle, &eco2);
printf("eCO2: %d\n", eco2);

i2cClose(handle);
gpioTerminate();

return 0;
}
