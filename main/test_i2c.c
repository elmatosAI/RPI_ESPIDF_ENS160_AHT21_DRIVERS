#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>  // Include unistd.h for sleep

// Define VERBOSE to enable verbose mode
//#define VERBOSE

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <i2c_address> <register_address>\n", argv[0]);
        return 1;
    }

    int i2c_address = strtol(argv[1], NULL, 16);
    int register_address = strtol(argv[2], NULL, 16);

    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed\n");
        return 1;
    }

#ifdef VERBOSE
    printf("pigpio initialization successful\n");
#endif

    int handle = i2cOpen(1, i2c_address, 0);
    if (handle < 0) {
        printf("Failed to open I2C bus\n");
        gpioTerminate();
        return 1;
    }

#ifdef VERBOSE
    printf("I2C bus opened successfully with address 0x%02X\n", i2c_address);
#endif

    uint8_t data = 0xFF;
    int ret = i2cReadByteData(handle, register_address);
    if (ret < 0) {
        printf("Failed to read from register 0x%02X\n", register_address);
        i2cClose(handle);
        gpioTerminate();
        return 1;
    }

    data = (uint8_t)ret;
    printf("Read 0x%02X from register 0x%02X\n", data, register_address);

#ifdef VERBOSE
    printf("Closing I2C bus\n");
#endif

    i2cClose(handle);
    gpioTerminate();

#ifdef VERBOSE
    printf("pigpio terminated\n");
#endif

    return 0;
}