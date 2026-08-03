#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUZZER_PIN GPIO_NUM_15

void app_main(void)
{
    gpio_reset_pin(BUZZER_PIN);

    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(BUZZER_PIN, 1);
        printf("Buzzer ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(BUZZER_PIN, 0);
        printf("Buzzer OFF\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}