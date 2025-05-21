
#include "main.h"
#include "fm_transmitter.h"
#include "lcd_screen.h"


QueueHandle_t data_queue;

i2c_master_dev_handle_t i2c_dev;
SSD1306_t lcd_dev;


void app_main(void) {

    printf("Beginning application...\n");

    init_i2c_0(&i2c_dev);
    init_i2c_1(&lcd_dev, GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_0);
    init_gpio();
    ssd1306_init(&lcd_dev, 128, 64);

    vTaskDelay(pdMS_TO_TICKS(100));

    //init_fm(&i2c_dev);

    lcd_demo();

    while(1)
    {
        if (gpio_get_level(TUNE_UP_PIN)) {
            fm_frequency += 20;
            tune_fm_freq(&i2c_dev, fm_frequency);
            while (gpio_get_level(TUNE_UP_PIN)); // wait for button release
        }
        if (gpio_get_level(TUNE_DOWN_PIN)) {
            fm_frequency -= 20;
            tune_fm_freq(&i2c_dev, fm_frequency);
            while (gpio_get_level(TUNE_DOWN_PIN)); // wait for button release
            
        }
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }  

    printf("Exiting application...\n");

    return;
}


void init_i2c_0(i2c_master_dev_handle_t* i2c_dev) {

    printf("Initializing I2C...\n");

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source                      = I2C_CLK_SRC_DEFAULT,
        .i2c_port                        = DEFAULT_I2C_PORT,
        .sda_io_num                      = GPIO_NUM_21,
        .scl_io_num                      = GPIO_NUM_22,
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

    printf("Initialized I2C\n");

    return;
}


void init_gpio(void) {

    printf("Initializing GPIO...\n");

    uint64_t pin_bitMask = 0;
    pin_bitMask |= (1ULL << SI4713_RESET_PIN); // reset for FM transmitter

    gpio_config_t io_conf = {
        .pin_bit_mask = pin_bitMask, 
        .mode = GPIO_MODE_OUTPUT,              
        .pull_up_en = GPIO_PULLUP_DISABLE,     
        .pull_down_en = GPIO_PULLDOWN_DISABLE, 
        .intr_type = GPIO_INTR_DISABLE         
    };
    gpio_config(&io_conf);

    // Configure GPIO32 and GPIO33 as inputs
    uint64_t input_pin_mask = (1ULL << TUNE_UP_PIN) | (1ULL << TUNE_DOWN_PIN);

    gpio_config_t io_conf_input = {
        .pin_bit_mask = input_pin_mask,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf_input);

    printf("GPIO initialized\n");

    return;
}