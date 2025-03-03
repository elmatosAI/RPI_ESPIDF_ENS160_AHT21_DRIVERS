// program for testing ENS160 i2c sensor with a Raspberry Pi Model 2B
// include raspberry pi i2c library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// include pigpio library
#include <pigpio.h>
#include "ens160.h"
#include "publish.h"

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

    // Start the web server in a separate thread
    pthread_t web_server_thread;
    pthread_create(&web_server_thread, NULL, (void *)start_web_server, NULL);

// Call your ENS160 functions here
// set ENS160 device to standard mode
ENS160_MODE_SET(handle, ENS160_MODE_STANDARD);

// Wait for data to be ready
if (ENS160_DATA_READY(handle) != 0) {
    return 1;
}

// read AQI value from ENS160 device
uint8_t aqi;
ENS160_GET_AQI(handle, &aqi);
printf("AQI: %d (0x%02X)\n", aqi, aqi);

// Wait for data to be ready
if (ENS160_DATA_READY(handle) != 0) {
    return 1;
}
// read TVOC value from ENS160 device
uint16_t tvoc;
ENS160_GET_TVOC(handle, &tvoc);
printf("TVOC: %d (0x%04X)\n", tvoc, tvoc);

// Wait for data to be ready
if (ENS160_DATA_READY(handle) != 0) {
    return 1;
}

// read eCO2 value from ENS160 device
uint16_t eco2;
ENS160_GET_eCO2(handle, &eco2);
printf("eCO2: %d (0x%04X)\n", eco2, eco2);

// Update the web page with sensor data
update_web_data(aqi, tvoc, eco2);

i2cClose(handle);
gpioTerminate();

return 0;
}
