#pragma once

#include "adc_hal.h"
#include "esp_err.h"

esp_err_t adc_temperature_sensor_raw_to_celsius(int raw_value, int *temperature_c);
esp_err_t adc_temperature_sensor_read_celsius(const adc_hal_t *hal, int *temperature_c);
