// filepath: /workspaces/RPI_ESPIDF_ENS160_AHT21_DRIVERS/main/publish.h
#ifndef PUBLISH_H
#define PUBLISH_H

void start_web_server();
void update_web_data(int aqi, int tvoc, int eco2);

#endif // PUBLISH_H