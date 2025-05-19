
#include "i2s_send.h"

void i2s_send(void) {

    int raw_data[I2S_BUFFER_SIZE];
    size_t bytes_to_write = I2S_BUFFER_SIZE;
    size_t bytes_written = 0;

    if (tx_handle == NULL) {
        printf("I2S TX handle is null, exiting task\n");
        return;
    }

    i2s_channel_enable(tx_handle);

    while (1) {

        UBaseType_t queue_length = uxQueueMessagesWaiting(data_queue);
        if (queue_length >= I2S_BUFFER_SIZE) {
            consume_data(raw_data);
            for (int i = 1; i < I2S_BUFFER_SIZE; i++) {
                printf("%d ", raw_data[i]);
            }
        }

        if (i2s_channel_write(tx_handle, raw_data, bytes_to_write, &bytes_written, pdMS_TO_TICKS(100)) != ESP_OK) {
            printf("I2S write failed, %d bytes written\n", bytes_written);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // give the CPU back for a sec
    }

    i2s_channel_disable(tx_handle);
    i2s_del_channel(tx_handle);

    return;
}

void consume_data(int* raw_data) {
    int adc_reading;

    for (int i = 1; i < I2S_BUFFER_SIZE; i++)
    {
        if (xQueueReceive(data_queue, &adc_reading, portMAX_DELAY))
        {
            raw_data[i] = adc_reading;
        }
    }
}