#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_4

void app_main(void)
{
    // Configure LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Configure Push Button
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    while (1)
    {
        int buttonState = gpio_get_level(BUTTON_PIN);

        if(buttonState == 0)
        {
            gpio_set_level(LED_PIN, 1);
            printf("Button Pressed - LED ON\n");
        }
        else
        {
            gpio_set_level(LED_PIN, 0);
            printf("Button Released - LED OFF\n");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}