#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>  // Include unistd.h for sleep

// Define VERBOSE to enable verbose mode
//#define VERBOSE

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <i2c_address> <register_address> <num_bytes>\n", argv[0]);
        return 1;
    }

    int i2c_address = strtol(argv[1], NULL, 16);
    int register_address = strtol(argv[2], NULL, 16);
    int num_bytes = atoi(argv[3]);

    if (num_bytes <= 0) {
        printf("Number of bytes to read must be a positive integer.\n");
        return 1;
    }

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

    uint8_t* data = (uint8_t*)malloc(num_bytes);
    if (data == NULL) {
        printf("Failed to allocate memory\n");
        i2cClose(handle);
        gpioTerminate();
        return 1;
    }

    int ret = i2cReadI2CBlockData(handle, register_address, (char*)data, num_bytes);
    if (ret < 0) {
        printf("Failed to read from register 0x%02X\n", register_address);
        free(data);
        i2cClose(handle);
        gpioTerminate();
        return 1;
    }

    printf("Read %d bytes from register 0x%02X: ", num_bytes, register_address);
    for (int i = 0; i < num_bytes; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");

#ifdef VERBOSE
    printf("Closing I2C bus\n");
#endif

    free(data);
    i2cClose(handle);
    gpioTerminate();

#ifdef VERBOSE
    printf("pigpio terminated\n");
#endif

    return 0;
}