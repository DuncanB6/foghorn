
#include "fm_transmitter.h"

int test_fm(i2c_master_dev_handle_t* i2c_dev) {

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