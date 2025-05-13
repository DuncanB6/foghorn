#ifndef _FM_TRANSMITTER_H
#define _FM_TRANSMITTER_H

#include "main.h"

int init_fm(i2c_master_dev_handle_t* i2c_dev);
int tune_fm_freq(i2c_master_dev_handle_t* i2c_dev, uint16_t freq_kHz);
int set_tx_power(i2c_master_dev_handle_t* i2c_dev, uint8_t power, uint8_t antcap);
int set_property(i2c_master_dev_handle_t* i2c_dev, uint16_t property, uint16_t value);
int power_up(i2c_master_dev_handle_t* i2c_dev);
int get_rev(i2c_master_dev_handle_t* i2c_dev);
esp_err_t send_i2c_command(i2c_master_dev_handle_t* i2c_dev, uint8_t* command, size_t command_len, uint8_t* response, size_t response_len);

#define FM_FREQUENCY              9990

#endif
