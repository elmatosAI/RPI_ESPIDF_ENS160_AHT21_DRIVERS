#include "ens160.h"
#include <pigpio.h>

static uint8_t ens160_addr_local = 0x53;

static int i2c_master_read_slave_reg(int handle, uint8_t i2c_reg, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return 0;
    }
    int status = i2cWriteByteData(handle, i2c_reg, 0);
    if (status < 0) {
        return status;
    }
    status = i2cReadDevice(handle, (char*)data_rd, size);
    return status;
}

static int i2c_master_write_slave_reg(int handle, uint8_t i2c_reg, uint8_t* data_wr, size_t size)
{
    int status = i2cWriteByteData(handle, i2c_reg, data_wr[0]);
    if (status < 0) {
        return status;
    }
    if (size > 1) {
        status = i2cWriteDevice(handle, (char*)data_wr, size);
    }
    return status;
}

void ENS160_SET_ID(uint8_t ens160_addr){
    ens160_addr_local = ens160_addr;
}

int ENS160_MODE_SET(int handle, uint8_t mode){
    return i2c_master_write_slave_reg(handle, ENS160_REG_OPMODE, &mode, 1);
}

// function ENS160_DATA_READY: wait data to be ready and break after timeout
int ENS160_DATA_READY(int handle){
    uint8_t status = 0;
    int timeout = 1000; // 1 second timeout
    while (timeout > 0) {
        if (ENS160_GET_STATUS(handle, &status) == 0) {
            if (status & 0x01) {
                return 0;
            }
        }
        time_sleep(0.005); // 1ms delay
        timeout--;
    }
    return -1;
}

// function ENS160_GET_DEVICE_ID: read device ID from register 0x00 and 0x01
int ENS160_GET_DEVICE_ID(int handle, uint8_t* device_id){
    return i2c_master_read_slave_reg(handle, ENS160_REG_DEVICE_ID, device_id, 2);
}

int ENS160_READ_MODE(int handle, uint8_t* mode){
    return i2c_master_read_slave_reg(handle, ENS160_REG_OPMODE, mode, 1);
}

int ENS160_GET_STATUS(int handle, uint8_t* status){
    return i2c_master_read_slave_reg(handle, ENS160_REG_DATA_STATUS, status, 1);
}

int ENS160_GET_eCO2(int handle, uint16_t* eco2){
    uint8_t data[2] = {0};
    int ret = i2c_master_read_slave_reg(handle, ENS160_REG_DATA_ECO2, data, 2);
    *eco2 = (data[1]<<8)|data[0];
    return ret;
}

int ENS160_SET_TEMP_IN(int handle, float temp){
    uint8_t data[2] = {0};
    int16_t scaled_value = (int16_t)(temp * 64.0f);

    // Clamp the value to the valid range for 10-bit integer + 6-bit fractional part
    if (scaled_value > 32767) scaled_value = 32767; // Ensure it fits in int16_t
    else if (scaled_value < -32768) scaled_value = -32768;

    // Split into two 8-bit bytes
    data[0] = (uint8_t)((scaled_value >> 8) & 0xFF); // High byte
    data[1] = (uint8_t)(scaled_value & 0xFF);        // Low byte
    return i2c_master_write_slave_reg(handle, ENS160_REG_TEMP_IN, data, 2);
}

int ENS160_SET_RH_IN(int handle, float rh){
    uint8_t data[2] = {0};
    int16_t scaled_value = (int16_t)(rh * 512.0f);

    // Clamp the value to the valid range for 10-bit integer + 6-bit fractional part
    if (scaled_value > ((127 << 9) | 511)) { // Max value: 127.998046875%
        scaled_value = ((127 << 9) | 511);
    }

    // Split into two 8-bit bytes
    data[0] = (uint8_t)((scaled_value >> 8) & 0xFF); // High byte
    data[1] = (uint8_t)(scaled_value & 0xFF);        // Low byte
    return i2c_master_write_slave_reg(handle, ENS160_REG_RH_IN, data, 2);
}

int ENS160_GET_AQI(int handle, uint8_t* aqi){
    int ret = i2c_master_read_slave_reg(handle, ENS160_REG_AQI, aqi, 1);
    *aqi = *aqi & 0x07; // mask all but lowest 3 bits;
    return ret;
}

int ENS160_GET_TVOC(int handle, uint16_t* tvoc){
    uint8_t data[2] = {0};
    int ret = i2c_master_read_slave_reg(handle, ENS160_REG_TVOC, data, 2);
    *tvoc = (data[1]<<8)|data[0];
    return ret;
}