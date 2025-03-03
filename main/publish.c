// publish a lean web page showing data from the ENS160 sensor
// one function for sending data to the web page

// filepath: /workspaces/RPI_ESPIDF_ENS160_AHT21_DRIVERS/main/publish.c
#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void handle_request(struct mg_connection *nc, struct http_message *hm) {
    char data[100];
    snprintf(data, sizeof(data), "{\"aqi\": %d, \"tvoc\": %d, \"eco2\": %d}", 0, 0, 0); // Placeholder values
    mg_send_head(nc, 200, strlen(data), "Content-Type: application/json");
    mg_printf(nc, "%s", data);
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_REQUEST) {
        struct http_message *hm = (struct http_message *) ev_data;
        handle_request(nc, hm);
    }
}

void start_web_server() {
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = ".";  // Serve current directory
    s_http_server_opts.enable_directory_listing = "yes";

    printf("Starting web server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

void update_web_data(int aqi, int tvoc, int eco2) {
    // Update the data to be sent to the web page
    // This function can be called from main.c to update the sensor data
}