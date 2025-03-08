#include "publish_csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static FILE *csv_file = NULL;

void open_csv_file(const char *filename) {
    csv_file = fopen(filename, "w");
    if (csv_file == NULL) {
        perror("Failed to open CSV file");
        exit(EXIT_FAILURE);
    }

    // Write the header row
    fprintf(csv_file, "date-time,AQI,TVOC,ECO2\n");
}

void append_csv_data(int aqi, int tvoc, int eco2) {
    if (csv_file == NULL) {
        fprintf(stderr, "CSV file is not open\n");
        return;
    }

    // Get the current date-time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date_time[20];
    strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", t);

    // Write the data row
    fprintf(csv_file, "%s,%d,%d,%d\n", date_time, aqi, tvoc, eco2);
}

void close_csv_file() {
    if (csv_file != NULL) {
        fclose(csv_file);
        csv_file = NULL;
    }
}