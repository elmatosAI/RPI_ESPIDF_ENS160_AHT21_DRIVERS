#include "publish.h"
#include "publish_csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

static int running = 1;

void* web_server_thread_func(void* arg) {
    start_web_server();
    return NULL;
}

void signal_handler(int signum) {
    running = 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <interval_seconds> <csv_filename>\n", argv[0]);
        return 1;
    }

    int interval = atoi(argv[1]);
    if (interval <= 0) {
        printf("Interval must be a positive integer.\n");
        return 1;
    }

    const char *csv_filename = argv[2];

    // Initialize random number generator
    srand(time(NULL));

    // Open the CSV file
    open_csv_file(csv_filename);

    // Start the web server in a separate thread
    pthread_t web_server_thread;
    pthread_create(&web_server_thread, NULL, web_server_thread_func, NULL);

    // Register signal handler for graceful termination
    signal(SIGINT, signal_handler);

    while (running) {
        // Generate random values for AQI, TVOC, and eCO2
        int aqi = rand() % 5; // AQI rang50-5
        int tvoc = rand() % 1000; // TVOC range: 0-1000
        int eco2 = rand() % 5000; // eCO2 range: 0-5000

        // Update the web page with random sensor data
        update_web_data(aqi, tvoc, eco2);

        // Append the data to the CSV file
        append_csv_data(aqi, tvoc, eco2);

        // Sleep for the specified interval
        sleep(interval);
    }

    // Close the CSV file
    close_csv_file();

    // Wait for the web server thread to finish
    pthread_cancel(web_server_thread);
    pthread_join(web_server_thread, NULL);

    printf("Program terminated.\n");
    return 0;
}