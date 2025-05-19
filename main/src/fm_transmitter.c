
#include "fm_transmitter.h"


int init_fm(i2c_master_dev_handle_t* i2c_dev) {

    // Reset the FM board
    gpio_set_level(SI4713_RESET_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(SI4713_RESET_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    if (!power_up(i2c_dev)) {
        return false;
    }

    if (!get_rev(i2c_dev)) {
        return false;
    }

    set_property(i2c_dev, 0x0201, 32768); // crystal is 32.768
    set_property(i2c_dev, 0x2106, 0); // 74uS pre-emph (USA std)
    set_property(i2c_dev, 0x2204, 10); // max gain?
    set_property(i2c_dev, 0x2200, 0x0); // turn on limiter and AGC

    vTaskDelay(pdMS_TO_TICKS(250)); // let the fm oscillator spin up

    set_tx_power(i2c_dev, 115, 0); // dBuV, 88-115 max, 0 -> autotuning antenna
    tune_fm_freq(i2c_dev, FM_FREQUENCY); // tune to defined frequency

    return true;
}


int tune_fm_freq(i2c_master_dev_handle_t* i2c_dev, uint16_t freq_kHz) {

    uint8_t command[4];
    command[0] = 0x30;
    command[1] = 0;
    command[2] = freq_kHz >> 8;
    command[3] = freq_kHz;
    uint8_t reply[1];
    if (send_i2c_command(i2c_dev, command, sizeof(command), reply, sizeof(reply)) != ESP_OK) {
        return false;
    }

    return true;
}


int set_tx_power(i2c_master_dev_handle_t* i2c_dev, uint8_t power, uint8_t antcap) {

    uint8_t command[5];
    command[0] = 0x31;
    command[1] = 0;
    command[2] = 0;
    command[3] = power;
    command[4] = antcap;
    uint8_t reply[1];
    if (send_i2c_command(i2c_dev, command, sizeof(command), reply, sizeof(reply)) != ESP_OK) {
        return false;
    }

    return true;
}


int set_property(i2c_master_dev_handle_t* i2c_dev, uint16_t property, uint16_t value) {

    uint8_t command[6];
    command[0] = 0x12;
    command[1] = 0;
    command[2] = property >> 8;
    command[3] = property & 0xFF;
    command[4] = value >> 8;
    command[5] = value & 0xFF;
    uint8_t reply[1];
    if (send_i2c_command(i2c_dev, command, sizeof(command), reply, sizeof(reply)) != ESP_OK) {
        return false;
    }

    return true;
}

int power_up(i2c_master_dev_handle_t* i2c_dev) {

    uint8_t command[3] = {0x01, 0x12, 0x50};
    uint8_t reply[1];
    if (send_i2c_command(i2c_dev, command, sizeof(command), reply, sizeof(reply)) != ESP_OK) {
        return false;
    }

    return true;
}


int get_rev(i2c_master_dev_handle_t* i2c_dev) {

    uint8_t command[1] = {0x10};
    uint8_t reply[9];
    if (send_i2c_command(i2c_dev, command, sizeof(command), reply, sizeof(reply)) != ESP_OK) {
        return false;
    }

    return true;
}



esp_err_t send_i2c_command(i2c_master_dev_handle_t* i2c_dev, uint8_t* command, size_t command_len, uint8_t* response, size_t response_len) {
    
    esp_err_t ret;

    ret = i2c_master_transmit(*i2c_dev, command, command_len, 1000/portTICK_PERIOD_MS); // 1s timeout
    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "Transmit failed: %s", esp_err_to_name(ret));
        return ret;
    }

    vTaskDelay(pdMS_TO_TICKS(10));

    ret = i2c_master_receive(*i2c_dev, response, response_len, 1000/portTICK_PERIOD_MS); // 1s timeout
    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "Receive failed: %s", esp_err_to_name(ret));
        return ret;
    }

    vTaskDelay(pdMS_TO_TICKS(100)); 

    if (DEBUG) {
        for (size_t j = 0; j < response_len; j++) {
            printf("0x%02X\n", response[j]);
        }
    }
    
    return ret;
}