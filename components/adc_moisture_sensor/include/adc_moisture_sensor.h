#pragma once

#include "adc_hal.h"
#include "esp_err.h"

esp_err_t adc_moisture_sensor_raw_to_percent(int raw_value, int *percent);
esp_err_t adc_moisture_sensor_read_percent(const adc_hal_t *hal, int *percent);
