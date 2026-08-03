#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RED_PIN     GPIO_NUM_25
#define GREEN_PIN   GPIO_NUM_26
#define BLUE_PIN    GPIO_NUM_27

void app_main(void)
{
    gpio_reset_pin(RED_PIN);
    gpio_reset_pin(GREEN_PIN);
    gpio_reset_pin(BLUE_PIN);

    gpio_set_direction(RED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        // RED ON
        gpio_set_level(RED_PIN, 1);
        gpio_set_level(GREEN_PIN, 0);
        gpio_set_level(BLUE_PIN, 0);
        printf("RED ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        // GREEN ON
        gpio_set_level(RED_PIN, 0);
        gpio_set_level(GREEN_PIN, 1);
        gpio_set_level(BLUE_PIN, 0);
        printf("GREEN ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        // BLUE ON
        gpio_set_level(RED_PIN, 0);
        gpio_set_level(GREEN_PIN, 0);
        gpio_set_level(BLUE_PIN, 1);
        printf("BLUE ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        // ALL OFF
        gpio_set_level(RED_PIN, 0);
        gpio_set_level(GREEN_PIN, 0);
        gpio_set_level(BLUE_PIN, 0);
        printf("ALL OFF\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}