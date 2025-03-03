#include "publish.h"
#include <stdlib.h>
#include <time.h>

int main() {
    // Initialize random number generator
    srand(time(NULL));

    // Generate random values for AQI, TVOC, and eCO2
    int aqi = rand() % 5; // AQI range: 0-5
    int tvoc = rand() % 1000; // TVOC range: 0-1000
    int eco2 = rand() % 5000; // eCO2 range: 0-5000

    // Update the web page with random sensor data
    update_web_data(aqi, tvoc, eco2);

    // Start the web server
    start_web_server();
    return 0;
}