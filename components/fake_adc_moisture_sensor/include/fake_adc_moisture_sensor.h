#pragma once

#include <stddef.h>

#include "adc_hal.h"
#include "esp_err.h"

#define FAKE_ADC_MOISTURE_SENSOR_MAX_READS 8

typedef struct {
    int values[FAKE_ADC_MOISTURE_SENSOR_MAX_READS];
    size_t value_count;
    size_t read_count;
    esp_err_t read_error;
} fake_adc_moisture_sensor_t;

void fake_adc_moisture_sensor_init(fake_adc_moisture_sensor_t *fake);
adc_hal_t fake_adc_moisture_sensor_as_adc_hal(fake_adc_moisture_sensor_t *fake);
void fake_adc_moisture_sensor_set_read_values(fake_adc_moisture_sensor_t *fake,
        const int *values,
        size_t value_count);
void fake_adc_moisture_sensor_set_read_error(fake_adc_moisture_sensor_t *fake, esp_err_t error);
