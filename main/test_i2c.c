#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>  // Include unistd.h for sleep

// Define VERBOSE to enable verbose mode
//#define VERBOSE

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: %s <i2c_address> <operation> <register_address> <num_bytes> [data_to_write]\n", argv[0]);
        return 1;
    }

    int i2c_address = strtol(argv[1], NULL, 16);
    char operation = argv[2][0];
    int register_address = strtol(argv[3], NULL, 16);
    int num_bytes = atoi(argv[4]);

    if (operation != 'r' && operation != 'w') {
        printf("Operation must be 'r' for read or 'w' for write.\n");
        return 1;
    }

    if (num_bytes <= 0) {
        printf("Number of bytes must be a positive integer.\n");
        return 1;
    }

    if (operation == 'w' && argc != 5 + num_bytes) {
        printf("For write operation, provide %d bytes of data to write.\n", num_bytes);
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

    if (operation == 'r') {
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

        free(data);
    } else if (operation == 'w') {
        uint8_t* data = (uint8_t*)malloc(num_bytes);
        if (data == NULL) {
            printf("Failed to allocate memory\n");
            i2cClose(handle);
            gpioTerminate();
            return 1;
        }

        for (int i = 0; i < num_bytes; i++) {
            data[i] = strtol(argv[5 + i], NULL, 16);
        }

        int ret = i2cWriteI2CBlockData(handle, register_address, (char*)data, num_bytes);
        if (ret < 0) {
            printf("Failed to write to register 0x%02X\n", register_address);
            free(data);
            i2cClose(handle);
            gpioTerminate();
            return 1;
        }

        printf("Wrote %d bytes to register 0x%02X: ", num_bytes, register_address);
        for (int i = 0; i < num_bytes; i++) {
            printf("0x%02X ", data[i]);
        }
        printf("\n");

        free(data);
    }

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