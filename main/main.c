#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "ens160.h"
#include "aht21.h"

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif



#define AHT21_ID    0x38

#define SDA_PIN 8
#define SCL_PIN 9

static const char *TAG = "i2cscanner";
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void task(void *ignore)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);

    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    uint8_t data[2] = {0};
    uint8_t datawr[2] = {0};
    //size_t count = 2;
    ENS160_SET_ID(0x53);
    ENS160_MODE_SET(I2C_NUM_0, ENS160_MODE_STANDARD);
    AHT21_CALIBRATION(I2C_NUM_0);
    while (1)
    {
        uint16_t data16 = 0;
        uint8_t  data8  = 0;
        float temp = 0;
        float rh = 0;
        AHT21_TRIGGER_MEASUREMENT(I2C_NUM_0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        ENS160_GET_eCO2(I2C_NUM_0, &data16);
        printf("eCO2: %d ,", data16);
        ENS160_GET_TVOC(I2C_NUM_0, &data16);
        printf("TVOC: %d ,", data16);
        ENS160_GET_AQI(I2C_NUM_0, &data8);
        printf("AQI: %d\n", data8);
        AHT21_READ_TEMP(I2C_NUM_0, &temp);
        printf("Temp: %f\n", temp);
        AHT21_READ_RH(I2C_NUM_0, &rh);
        printf("RH: %f\n", rh);
        vTaskDelay(800 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    // Start task
    xTaskCreatePinnedToCore(task, TAG, configMINIMAL_STACK_SIZE * 10, NULL, 5, NULL, APP_CPU_NUM);
}