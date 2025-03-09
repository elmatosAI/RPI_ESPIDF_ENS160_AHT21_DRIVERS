#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>  // Include unistd.h for sleep
#include "ens160.h"

int main() {
    // Initialize pigpio library
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

    // Insert 200ms delay for I2C device to stabilize
    time_sleep(0.2);

    // Set ENS160 device to standard mode
    ENS160_MODE_SET(handle, ENS160_MODE_STANDARD);

    // Wait for data to be ready
    if (ENS160_DATA_READY(handle) != 0) {
        printf("Data not ready\n");
        i2cClose(handle);
        gpioTerminate();
        return 1;
    }

    // Read AQI value from ENS160 device
    uint8_t aqi;
    ENS160_GET_AQI(handle, &aqi);
    printf("AQI: %d (0x%02X)\n", aqi, aqi);

    i2cClose(handle);
    gpioTerminate();

    return 0;
}