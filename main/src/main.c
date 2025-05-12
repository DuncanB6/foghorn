
#include "main.h"


void app_main(void)
{
    printf("Beginning application...\n");

    init_i2c(&i2c_dev);

    printf("Exiting application...\n");

    return;
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

    return;
}