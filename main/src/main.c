
#include "main.h"
#include "fm_transmitter.h"
#include "i2s_send.h"


QueueHandle_t data_queue;

i2c_master_dev_handle_t i2c_dev;
i2s_chan_handle_t tx_handle;
esp_timer_handle_t adc_timer;


void app_main(void) {

    printf("Beginning application...\n");

    data_queue   = xQueueCreate(DATA_QUEUE_DEPTH, sizeof(int));

    init_i2c(&i2c_dev);
    init_i2s(&tx_handle);
    init_gpio();
    //init_acquisition_timer();

    vTaskDelay(pdMS_TO_TICKS(100));

    init_fm(&i2c_dev);

    //xTaskCreate(i2s_send, "i2s_send", 4096, NULL, 5, NULL);

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }  

    printf("Exiting application...\n");

    return;
}


void init_acquisition_timer(void) {

    printf("Initializing DAQ timer...\n");

    const esp_timer_create_args_t timer_args = 
    {
        .callback = &acquire_sample, 
        .name = "adc_timer"
    };
    
    esp_timer_create(&timer_args, &adc_timer);
    esp_timer_start_periodic(adc_timer, ADC_SAMPLING_PERIOD_IN_US);  

    printf("DAQ timer initialized\n");

    return;
}


void IRAM_ATTR acquire_sample(void *arg) {
    
    int adc_reading = adc1_get_raw(ADC1_CHANNEL_0); // get adc reading from mic pin
    
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
    // if (xQueueSendFromISR(data_queue, (int*)&adc_reading, &xHigherPriorityTaskWoken) != pdPASS) {
    //     //printf("Queue is full, packet dropped\n");
    // }
    // if (xHigherPriorityTaskWoken == pdTRUE) {
    //     portYIELD_FROM_ISR();
    // }

    // UBaseType_t queue_length = uxQueueMessagesWaiting(data_queue);
    // printf("%u\n", queue_length);

    return;
}


void init_i2s(i2s_chan_handle_t* tx_handle) {

    printf("Initializing I2S...\n");
    
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_new_channel(&chan_cfg, tx_handle, NULL);

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(I2S_FREQUENCY),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = GPIO_NUM_4,
            .ws = GPIO_NUM_5,
            .dout = GPIO_NUM_18,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    i2s_channel_init_std_mode(*tx_handle, &std_cfg);

    printf("I2S initialized\n");

    return;
}


void init_i2c(i2c_master_dev_handle_t* i2c_dev) {

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

    // ADC for microphone
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

    printf("GPIO initialized\n");

    return;
}