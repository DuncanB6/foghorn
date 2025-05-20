#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

extern QueueHandle_t data_queue;

extern i2c_master_dev_handle_t i2c_dev;

void init_i2c(i2c_master_dev_handle_t* i2c_dev);
void init_gpio(void);

#define DEBUG                     true

// I2C specific
#define DEFAULT_SCL_SPEED_IN_HZ   (400000)          // 500KHz
#define DEFAULT_GLITCH_IGNORE_CNT (7)
#define DEFAULT_XFER_TIMEOUT_MS   (-1)
#define DEFAULT_I2C_PORT          (0)
#define DEFAULT_I2C_INTR_PRIORITY (0)
#define SI4713_I2C_ADDR           (0x63)            // for SI4713 FM transmitter

#define SI4713_RESET_PIN          GPIO_NUM_23       // for SI4713 FM transmitter
#define TUNE_UP_PIN               GPIO_NUM_32
#define TUNE_DOWN_PIN             GPIO_NUM_33

extern int fm_frequency;


#endif