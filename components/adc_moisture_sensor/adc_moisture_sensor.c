#include "adc_moisture_sensor.h"

#define ADC_MOISTURE_RAW_MIN 0
#define ADC_MOISTURE_RAW_MAX 4095
#define ADC_MOISTURE_PERCENT_MAX 100

esp_err_t adc_moisture_sensor_raw_to_percent(int raw_value, int *percent)
{
    if (percent == NULL || raw_value < ADC_MOISTURE_RAW_MIN || raw_value > ADC_MOISTURE_RAW_MAX) {
        return ESP_ERR_INVALID_ARG;
    }

    *percent = (raw_value * ADC_MOISTURE_PERCENT_MAX) / ADC_MOISTURE_RAW_MAX;
    return ESP_OK;
}

esp_err_t adc_moisture_sensor_read_percent(const adc_hal_t *hal, int *percent)
{
    if (percent == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    int raw_value = 0;
    esp_err_t err = adc_hal_read_raw(hal, &raw_value);
    if (err != ESP_OK) {
        return err;
    }

    return adc_moisture_sensor_raw_to_percent(raw_value, percent);
}
