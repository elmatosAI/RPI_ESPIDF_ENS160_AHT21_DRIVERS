#ifndef AHT21_H // include guard
#define AHT21_H


#pragma once
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

void AHT21_SET_ID(uint8_t aht21_addr);
esp_err_t AHT21_TRIGGER_MEASUREMENT(i2c_port_t i2c_num);
esp_err_t AHT21_CALIBRATION(i2c_port_t i2c_num);
esp_err_t AHT21_RESET(i2c_port_t i2c_num);
esp_err_t AHT21_READ_TEMP(i2c_port_t i2c_num, float* temp);
esp_err_t AHT21_READ_RH(i2c_port_t i2c_num, float* rHum);

#endif