#ifndef ENS160_H // include guard
#define ENS160_H

// include Raspberry Pi i2c library smbus2
#include <stdint.h>
#include <stdio.h>

#define ENS160_REG_OPMODE       0x10
#define ENS160_REG_TEMP_IN      0x13
#define ENS160_REG_RH_IN        0x15
#define ENS160_REG_DATA_STATUS  0x20
#define ENS160_REG_AQI          0x21
#define ENS160_REG_TVOC         0x22
#define ENS160_REG_DATA_ECO2    0x24

#define ENS160_MODE_SLEEP       0x00
#define ENS160_MODE_IDLE        0x01
#define ENS160_MODE_STANDARD    0x02


void ENS160_SET_ID(uint8_t ens160_addr);
esp_err_t ENS160_MODE_SET(i2c_port_t i2c_num, uint8_t mode);
esp_err_t ENS160_READ_MODE(i2c_port_t i2c_num, uint8_t* mode);
esp_err_t ENS160_GET_STATUS(i2c_port_t i2c_num, uint8_t* status);
esp_err_t ENS160_GET_eCO2(i2c_port_t i2c_num, uint16_t* eco2);
esp_err_t ENS160_SET_TEMP_IN(i2c_port_t i2c_num, float temp);
esp_err_t ENS160_SET_RH_IN(i2c_port_t i2c_num, float rh);
esp_err_t ENS160_GET_AQI(i2c_port_t i2c_num, uint8_t* aqi);
esp_err_t ENS160_GET_TVOC(i2c_port_t i2c_num, uint16_t* tvoc);
#endif