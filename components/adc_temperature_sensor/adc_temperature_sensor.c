#include "adc_temperature_sensor.h"

#define ADC_TEMPERATURE_RAW_MIN 0
#define ADC_TEMPERATURE_RAW_MAX 4095
#define ADC_TEMPERATURE_MIN_C   (-40)
#define ADC_TEMPERATURE_MAX_C   85

esp_err_t adc_temperature_sensor_raw_to_celsius(int raw_value, int *temperature_c)
{
    if (temperature_c == NULL || raw_value < ADC_TEMPERATURE_RAW_MIN || raw_value > ADC_TEMPERATURE_RAW_MAX) {
        return ESP_ERR_INVALID_ARG;
    }

    int range_c = ADC_TEMPERATURE_MAX_C - ADC_TEMPERATURE_MIN_C;
    *temperature_c = ADC_TEMPERATURE_MIN_C + (raw_value * range_c) / ADC_TEMPERATURE_RAW_MAX;
    return ESP_OK;
}

esp_err_t adc_temperature_sensor_read_celsius(const adc_hal_t *hal, int *temperature_c)
{
    if (temperature_c == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    int raw_value = 0;
    esp_err_t err = adc_hal_read_raw(hal, &raw_value);
    if (err != ESP_OK) {
        return err;
    }

    return adc_temperature_sensor_raw_to_celsius(raw_value, temperature_c);
}
