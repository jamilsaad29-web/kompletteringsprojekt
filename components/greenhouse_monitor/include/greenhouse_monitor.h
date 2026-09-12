#pragma once

#include <stdbool.h>

#include "adc_hal.h"
#include "esp_err.h"

typedef enum {
    GREENHOUSE_SOIL_INVALID = 0,
    GREENHOUSE_SOIL_DRY,
    GREENHOUSE_SOIL_OK,
    GREENHOUSE_SOIL_WET,
} greenhouse_soil_status_t;

typedef enum {
    GREENHOUSE_TEMPERATURE_INVALID = 0,
    GREENHOUSE_TEMPERATURE_LOW,
    GREENHOUSE_TEMPERATURE_OK,
    GREENHOUSE_TEMPERATURE_HIGH,
    GREENHOUSE_TEMPERATURE_CRITICAL,
} greenhouse_temperature_status_t;

typedef enum {
    GREENHOUSE_ACTION_IDLE = 0,
    GREENHOUSE_ACTION_WATER,
    GREENHOUSE_ACTION_ALARM,
} greenhouse_action_t;

typedef struct {
    int temperature_c;
    int soil_moisture_percent;
} greenhouse_reading_t;

typedef struct {
    greenhouse_reading_t reading;
    greenhouse_action_t action;
    esp_err_t read_error;
} greenhouse_monitor_result_t;

bool greenhouse_reading_is_valid(greenhouse_reading_t reading);
greenhouse_monitor_result_t greenhouse_monitor_update(const adc_hal_t *moisture_sensor_hal,
        const adc_hal_t *temperature_sensor_hal);
greenhouse_soil_status_t greenhouse_soil_status(int soil_moisture_percent);
greenhouse_temperature_status_t greenhouse_temperature_status(int temperature_c);
bool greenhouse_should_water(greenhouse_reading_t reading);
greenhouse_action_t greenhouse_decide_action(greenhouse_reading_t reading);
const char *greenhouse_action_to_string(greenhouse_action_t action);
const char *greenhouse_soil_status_to_string(greenhouse_soil_status_t status);
