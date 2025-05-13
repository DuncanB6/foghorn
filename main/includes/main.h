#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>

#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static i2c_master_dev_handle_t i2c_dev;

void init_i2c(i2c_master_dev_handle_t* i2c_dev);
void init_gpio(void);
void test_fm(i2c_master_dev_handle_t* i2c_dev);
esp_err_t send_i2c_command(i2c_master_dev_handle_t* i2c_dev, uint8_t* command, size_t command_len, uint8_t* response, size_t response_len);

// I2C specific
#define DEFAULT_I2C_SDA_PIN       GPIO_NUM_1
#define DEFAULT_I2C_SCL_PIN       GPIO_NUM_0
#define DEFAULT_SCL_SPEED_IN_HZ   (400000)          // 500KHz
#define DEFAULT_GLITCH_IGNORE_CNT (7)
#define DEFAULT_XFER_TIMEOUT_MS   (-1)
#define DEFAULT_I2C_PORT          (0)
#define DEFAULT_I2C_INTR_PRIORITY (0)

// FM transmitter specific
#define SI4713_I2C_ADDR           (0x63)
#define SI4713_RESET_PIN          GPIO_NUM_5
#define I2C_ADDR                  0x63


#endif