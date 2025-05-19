#include "microphone.h"

void microphone(void) {

    while (1) {
        int adc_reading = adc1_get_raw(ADC1_CHANNEL_0);  // Read raw ADC value
        if (xQueueSend(data_queue, (int*)&adc_reading, (TickType_t)10) != pdPASS) {
            printf("[WARNING] Failed to send LCD to message queue!\n");
        }
        //printf("%d\n", adc_reading);
        vTaskDelay(pdMS_TO_TICKS(100));  // Delay for 1 second
    }
}