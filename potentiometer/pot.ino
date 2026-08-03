#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"

#define POT_CHANNEL ADC_CHANNEL_6     // GPIO34

void app_main(void)
{
    adc_oneshot_unit_handle_t adc_handle;

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };

    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };

    adc_oneshot_config_channel(adc_handle, POT_CHANNEL, &config);

    int adc_raw;

    while (1)
    {
        adc_oneshot_read(adc_handle, POT_CHANNEL, &adc_raw);

        printf("Potentiometer Value = %d\n", adc_raw);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}