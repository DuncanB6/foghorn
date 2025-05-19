
#include "i2s_send.h"

void i2s_send(void) {

    uint32_t proc_data[I2S_BUFFER_SIZE];
    int raw_data[I2S_BUFFER_SIZE];
    float min;
    float max;
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
            min = raw_data[0];
            max = raw_data[0];

            // find min and max values
            for (size_t i = 1; i < I2S_BUFFER_SIZE; ++i) {
                if (raw_data[i] < min) min = raw_data[i];
                if (raw_data[i] > max) max = raw_data[i];
            }

            // normalize values between 0 and UINT32_MAX
            for (size_t i = 0; i < I2S_BUFFER_SIZE; ++i) {
                if (max - min == 0.0f) {
                    proc_data[i] = 0; // Avoid division by zero if all values are the same
                } else {
                    float normalized = (raw_data[i] - min) / (max - min);
                    proc_data[i] = (uint32_t)(normalized * UINT32_MAX);
                }
            }

            if (i2s_channel_write(tx_handle, proc_data, bytes_to_write, &bytes_written, pdMS_TO_TICKS(100)) != ESP_OK) {
                printf("I2S write failed, %d bytes written\n", bytes_written);
            }
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