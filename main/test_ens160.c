#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>  // Include unistd.h for sleep
#include <stdlib.h>  // Include stdlib.h for atoi
#include "ens160.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <sleep_time_seconds>\n", argv[0]);
        return 1;
    }

    int sleep_time = atoi(argv[1]);
    if (sleep_time <= 0) {
        printf("Sleep time must be a positive integer in seconds.\n");
        return 1;
    }

    // Initialize pigpio library
    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed\n");
        return 1;
    }else{
        printf("pigpio initialization successful\n");
    }

    int handle = i2cOpen(1, 0x53, 0); // Open I2C bus 1 with device address 0x53
    if (handle < 0) {
        printf("Failed to open I2C bus\n");
        gpioTerminate();
        return 1;
    }else{
        printf("I2C bus opened successfully with address 0x53\n");
    }

    // insert sleep_time delay for I2C device to stabilize
    sleep(sleep_time);

    // In I2C address 0x53, verify the device ID is 0x60 in register 0x00 and 0x01 in register 0x01
    uint16_t device_id = 0xFFFF;
    ENS160_GET_DEVICE_ID(handle, &device_id);
    printf("Device ID: 0x%04X\n", device_id);
    // if the device ID is not 0x0160, exit the program
    if (device_id != 0x0160) {
        printf("Device ID is not 0x0160\n");
        i2cClose(handle);
        gpioTerminate();
        return 1;
    }
    
    // Insert initial delay for I2C device to stabilize
    sleep(sleep_time);

    // Set ENS160 device to standard mode
    ENS160_MODE_SET(handle, ENS160_MODE_STANDARD);

    // Insert delay for I2C device to stabilize
    sleep(sleep_time);

    // Wait for data to be ready
    if (ENS160_DATA_READY(handle) != 0) {
        printf("Data not ready\n");
        i2cClose(handle);
        gpioTerminate();
        return 1;
    }else{
        printf("Data ready\n");
    }

    // Sleep for the specified interval before the next ac
    sleep(sleep_time);

    // Read AQI value from ENS160 device
    uint8_t aqi;
    ENS160_GET_AQI(handle, &aqi);
    printf("AQI: %d (0x%02X)\n", aqi, aqi);

    // Sleep for the specified interval before the next access
    sleep(sleep_time);

    i2cClose(handle);
    gpioTerminate();

    return 0;
}