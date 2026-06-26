#include <stdio.h>

#include "esp_err.h"
#include "greenhouse_monitor.h"

typedef struct {
    const int *raw_values;
    size_t value_count;
    size_t next_value;
} demo_adc_sequence_t;

static esp_err_t demo_adc_sequence_read_raw(void *ctx, int *raw_value)
{
    demo_adc_sequence_t *sequence = (demo_adc_sequence_t *)ctx;
    if (sequence == NULL || raw_value == NULL || sequence->raw_values == NULL || sequence->value_count == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t index = sequence->next_value;
    if (index >= sequence->value_count) {
        index = sequence->value_count - 1;
    }

    *raw_value = sequence->raw_values[index];
    sequence->next_value++;
    return ESP_OK;
}

/*
 * Main only shows a demo. The interesting things happens in the components.
*/
void app_main(void)
{
    const int moisture_raw_values[] = {
        2129,
        983,
        2539,
        737,
        3640,
    };
    const int temperature_raw_values[] = {
        2030,
        2079,
        2300,
        2325,
        2500,
    };
    const size_t value_count = sizeof(moisture_raw_values) / sizeof(moisture_raw_values[0]);
    demo_adc_sequence_t moisture_sensor = {
        .raw_values = moisture_raw_values,
        .value_count = value_count,
        .next_value = 0,
    };
    demo_adc_sequence_t temperature_sensor = {
        .raw_values = temperature_raw_values,
        .value_count = sizeof(temperature_raw_values) / sizeof(temperature_raw_values[0]),
        .next_value = 0,
    };
    const adc_hal_t moisture_hal = {
        .ctx = &moisture_sensor,
        .read_raw = demo_adc_sequence_read_raw,
    };
    const adc_hal_t temperature_hal = {
        .ctx = &temperature_sensor,
        .read_raw = demo_adc_sequence_read_raw,
    };

    printf("Greenhouse monitor started\n");

    for (size_t i = 0; i < value_count; ++i) {
        greenhouse_monitor_result_t result = greenhouse_monitor_update(&moisture_hal, &temperature_hal);
        greenhouse_soil_status_t soil_status = greenhouse_soil_status(result.reading.soil_moisture_percent);

        printf("sample=%u err=%s temp=%dC soil=%d%% soil_status=%s action=%s\n",
               (unsigned)i + 1,
               result.read_error == ESP_OK ? "ok" : "read_failed",
               result.reading.temperature_c,
               result.reading.soil_moisture_percent,
               greenhouse_soil_status_to_string(soil_status),
               greenhouse_action_to_string(result.action));
    }
}
