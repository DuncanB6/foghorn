
#include "main.h"


void app_main(void) {

    printf("Beginning application...\n");

    init_i2c(&i2c_dev);
    init_gpio();

    test_fm(&i2c_dev);

    printf("Exiting application...\n");

    return;
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


void init_i2c(i2c_master_dev_handle_t* i2c_dev) {

    printf("Initializing I2C...\n");

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source                      = I2C_CLK_SRC_RC_FAST,
        .i2c_port                        = DEFAULT_I2C_PORT,
        .sda_io_num                      = DEFAULT_I2C_SDA_PIN,
        .scl_io_num                      = DEFAULT_I2C_SCL_PIN,
        .glitch_ignore_cnt               = DEFAULT_GLITCH_IGNORE_CNT,
        .flags.enable_internal_pullup    = true,
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SI4713_I2C_ADDR,
        .scl_speed_hz = DEFAULT_SCL_SPEED_IN_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, i2c_dev));

    printf("I2C initialized\n");

    return;
}


void init_gpio(void) {

    printf("Initializing GPIO...\n");

    uint64_t pin_bitMask = 0;
    pin_bitMask |= (1ULL << SI4713_RESET_PIN);

    gpio_config_t io_conf = {
        .pin_bit_mask = pin_bitMask, 
        .mode = GPIO_MODE_OUTPUT,              
        .pull_up_en = GPIO_PULLUP_DISABLE,     
        .pull_down_en = GPIO_PULLDOWN_DISABLE, 
        .intr_type = GPIO_INTR_DISABLE         
    };
    gpio_config(&io_conf);

    printf("GPIO initialized\n");

    return;
}