// program for testing ENS160 i2c sensor with a Raspberry Pi Model 2B
// include raspberry pi i2c library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pigpio.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "ens160.h"
#include "publish.h"
#include "publish_csv.h"

static int running = 1;

void signal_handler(int signum) {
    running = 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <csv_filename>\n", argv[0]);
        return 1;
    }

    const char *csv_filename = argv[1];

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

    // Start the web server in a separate thread
    pthread_t web_server_thread;
    pthread_create(&web_server_thread, NULL, (void *)start_web_server, NULL);

    // Open the CSV file
    open_csv_file(csv_filename);

    // Register signal handler for graceful termination
    signal(SIGINT, signal_handler);

    while (running) {
        // Set ENS160 device to standard mode
        ENS160_MODE_SET(handle, ENS160_MODE_STANDARD);

        // Wait for data to be ready
        if (ENS160_DATA_READY(handle) != 0) {
            continue;
        }

        // Read AQI value from ENS160 device
        uint8_t aqi;
        ENS160_GET_AQI(handle, &aqi);
        printf("AQI: %d (0x%02X)\n", aqi, aqi);

        // Wait for data to be ready
        if (ENS160_DATA_READY(handle) != 0) {
            continue;
        }

        // Read TVOC value from ENS160 device
        uint16_t tvoc;
        ENS160_GET_TVOC(handle, &tvoc);
        printf("TVOC: %d (0x%04X)\n", tvoc, tvoc);

        // Wait for data to be ready
        if (ENS160_DATA_READY(handle) != 0) {
            continue;
        }

        // Read eCO2 value from ENS160 device
        uint16_t eco2;
        ENS160_GET_eCO2(handle, &eco2);
        printf("eCO2: %d (0x%04X)\n", eco2, eco2);

        // Update the web page with sensor data
        update_web_data(aqi, tvoc, eco2);

        // Append the data to the CSV file
        append_csv_data(aqi, tvoc, eco2);

        // Sleep for a short interval before reading the next set of data
        sleep(1);
    }

    // Close the CSV file
    close_csv_file();

    // Wait for the web server thread to finish
    pthread_cancel(web_server_thread);
    pthread_join(web_server_thread, NULL);

    i2cClose(handle);
    gpioTerminate();

    printf("Program terminated.\n");
    return 0;
}
