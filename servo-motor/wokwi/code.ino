#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define SERVO_PIN GPIO_NUM_18

void app_main(void)
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&ledc_channel);

    while (1)
    {
        // 0 Degree
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 410);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        printf("Servo : 0 Degree\n");
        vTaskDelay(pdMS_TO_TICKS(2000));

        // 90 Degree
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1230);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        printf("Servo : 90 Degree\n");
        vTaskDelay(pdMS_TO_TICKS(2000));

        // 180 Degree
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 2050);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        printf("Servo : 180 Degree\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}