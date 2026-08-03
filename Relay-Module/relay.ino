#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RELAY_PIN GPIO_NUM_23

void app_main(void)
{
    gpio_reset_pin(RELAY_PIN);

    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(RELAY_PIN, 1);
        printf("Relay ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(RELAY_PIN, 0);
        printf("Relay OFF\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}