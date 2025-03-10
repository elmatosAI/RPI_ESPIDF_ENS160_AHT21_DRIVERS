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
int ENS160_MODE_SET(int i2c_num, uint8_t mode);
int ENS160_DATA_READY(int i2c_num);
int ENS160_GET_DEVICE_ID(int i2c_num, uint8_t* device_id);
int ENS160_READ_MODE(int i2c_num, uint8_t* mode);
int ENS160_GET_STATUS(int i2c_num, uint8_t* status);
int ENS160_GET_eCO2(int i2c_num, uint16_t* eco2);
int ENS160_SET_TEMP_IN(int i2c_num, float temp);
int ENS160_SET_RH_IN(int i2c_num, float rh);
int ENS160_GET_AQI(int i2c_num, uint8_t* aqi);
int ENS160_GET_TVOC(int i2c_num, uint16_t* tvoc);
#endif