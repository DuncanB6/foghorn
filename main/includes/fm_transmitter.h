#ifndef _FM_TRANSMITTER_H
#define _FM_TRANSMITTER_H

#include "main.h"

int test_fm(i2c_master_dev_handle_t* i2c_dev);
int power_up(i2c_master_dev_handle_t* i2c_dev);
int get_rev(i2c_master_dev_handle_t* i2c_dev);
esp_err_t send_i2c_command(i2c_master_dev_handle_t* i2c_dev, uint8_t* command, size_t command_len, uint8_t* response, size_t response_len);

#endif
