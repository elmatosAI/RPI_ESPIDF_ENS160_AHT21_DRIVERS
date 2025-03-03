// publish a lean web page showing data from the ENS160 sensor
// one function for sending data to the web page

// filepath: /workspaces/RPI_ESPIDF_ENS160_AHT21_DRIVERS/main/publish.c
#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *s_http_port = "http://0.0.0.0:8000";
static int s_aqi = 0;
static int s_tvoc = 0;
static int s_eco2 = 0;

static void handle_request(struct mg_connection *nc, struct mg_http_message *hm) {
    char data[100];
    snprintf(data, sizeof(data), "{\"aqi\": %d, \"tvoc\": %d, \"eco2\": %d}", s_aqi, s_tvoc, s_eco2);
    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", data);
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        handle_request(nc, hm);
    }
}

void start_web_server() {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    mg_mgr_init(&mgr);
    nc = mg_http_listen(&mgr, s_http_port, ev_handler, &mgr);
    if (nc == NULL) {
        printf("Failed to create listener\n");
        return;
    }

    printf("Starting web server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

void update_web_data(int aqi, int tvoc, int eco2) {
    s_aqi = aqi;
    s_tvoc = tvoc;
    s_eco2 = eco2;
}