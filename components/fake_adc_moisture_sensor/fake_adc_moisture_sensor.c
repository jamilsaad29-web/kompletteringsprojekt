#include "fake_adc_moisture_sensor.h"

#include <string.h>

static esp_err_t fake_adc_moisture_sensor_read_raw(void *ctx, int *raw_value)
{
    fake_adc_moisture_sensor_t *fake = (fake_adc_moisture_sensor_t *)ctx;
    if (fake == NULL || raw_value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (fake->read_error != ESP_OK) {
        return fake->read_error;
    }

    size_t index = fake->read_count;
    if (index >= fake->value_count) {
        index = fake->value_count > 0 ? fake->value_count - 1 : 0;
    }

    *raw_value = fake->value_count > 0 ? fake->values[index] : 0;
    fake->read_count++;
    return ESP_OK;
}

void fake_adc_moisture_sensor_init(fake_adc_moisture_sensor_t *fake)
{
    if (fake == NULL) {
        return;
    }

    memset(fake, 0, sizeof(*fake));
    fake->read_error = ESP_OK;
}

adc_hal_t fake_adc_moisture_sensor_as_adc_hal(fake_adc_moisture_sensor_t *fake)
{
    return (adc_hal_t) {
        .ctx = fake,
        .read_raw = fake_adc_moisture_sensor_read_raw,
    };
}

void fake_adc_moisture_sensor_set_read_values(fake_adc_moisture_sensor_t *fake,
        const int *values,
        size_t value_count)
{
    if (fake == NULL || values == NULL) {
        return;
    }

    if (value_count > FAKE_ADC_MOISTURE_SENSOR_MAX_READS) {
        value_count = FAKE_ADC_MOISTURE_SENSOR_MAX_READS;
    }

    memcpy(fake->values, values, value_count * sizeof(values[0]));
    fake->value_count = value_count;
    fake->read_count = 0;
}

void fake_adc_moisture_sensor_set_read_error(fake_adc_moisture_sensor_t *fake, esp_err_t error)
{
    if (fake != NULL) {
        fake->read_error = error;
    }
}
