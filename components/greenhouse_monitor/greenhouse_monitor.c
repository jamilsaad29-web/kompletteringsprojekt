#include "greenhouse_monitor.h"

#include "adc_moisture_sensor.h"
#include "adc_temperature_sensor.h"

#define GREENHOUSE_MIN_TEMP_C         (-40)
#define GREENHOUSE_MAX_TEMP_C         85
#define GREENHOUSE_DRY_SOIL_LIMIT     35
#define GREENHOUSE_WET_SOIL_LIMIT     70
#define GREENHOUSE_CRITICAL_TEMP_C    35
#define GREENHOUSE_MIN_MOISTURE       0
#define GREENHOUSE_MAX_MOISTURE       100

bool greenhouse_reading_is_valid(greenhouse_reading_t reading)
{
    return reading.temperature_c >= GREENHOUSE_MIN_TEMP_C &&
           reading.temperature_c <= GREENHOUSE_MAX_TEMP_C &&
           reading.soil_moisture_percent >= GREENHOUSE_MIN_MOISTURE &&
           reading.soil_moisture_percent <= GREENHOUSE_MAX_MOISTURE;
}

greenhouse_monitor_result_t greenhouse_monitor_update(const adc_hal_t *moisture_sensor_hal,
        const adc_hal_t *temperature_sensor_hal)
{
    greenhouse_monitor_result_t result = {
        .reading = {
            .temperature_c = 0,
            .soil_moisture_percent = 0,
        },
        .action = GREENHOUSE_ACTION_ALARM,
        .read_error = ESP_OK,
    };

    result.read_error = adc_moisture_sensor_read_percent(moisture_sensor_hal,
                        &result.reading.soil_moisture_percent);
    if (result.read_error != ESP_OK) {
        return result;
    }

    result.read_error = adc_temperature_sensor_read_celsius(temperature_sensor_hal,
                        &result.reading.temperature_c);
    if (result.read_error != ESP_OK) {
        return result;
    }

    result.action = greenhouse_decide_action(result.reading);
    return result;
}

greenhouse_soil_status_t greenhouse_soil_status(int soil_moisture_percent)
{
    if (soil_moisture_percent < GREENHOUSE_MIN_MOISTURE ||
        soil_moisture_percent > GREENHOUSE_MAX_MOISTURE) {
        return GREENHOUSE_SOIL_INVALID;
    }

    if (soil_moisture_percent < GREENHOUSE_DRY_SOIL_LIMIT) {
        return GREENHOUSE_SOIL_DRY;
    }

    if (soil_moisture_percent > GREENHOUSE_WET_SOIL_LIMIT) {
        return GREENHOUSE_SOIL_WET;
    }

    return GREENHOUSE_SOIL_OK;
}

greenhouse_temperature_status_t greenhouse_temperature_status(int temperature_c)
{
    if (temperature_c < GREENHOUSE_MIN_TEMP_C ||
        temperature_c > GREENHOUSE_MAX_TEMP_C) {
        return GREENHOUSE_TEMPERATURE_INVALID;
    }

    if (temperature_c < 15) {
        return GREENHOUSE_TEMPERATURE_LOW;
    }

    if (temperature_c <= 27) {
        return GREENHOUSE_TEMPERATURE_OK;
    }

    if (temperature_c <= 34) {
        return GREENHOUSE_TEMPERATURE_HIGH;
    }

    return GREENHOUSE_TEMPERATURE_CRITICAL;
}

greenhouse_action_t greenhouse_decide_action(greenhouse_reading_t reading)
{
    if (!greenhouse_reading_is_valid(reading)) {
        return GREENHOUSE_ACTION_ALARM;
    }

    if (reading.temperature_c >= GREENHOUSE_CRITICAL_TEMP_C) {
        return GREENHOUSE_ACTION_ALARM;
    }

    greenhouse_soil_status_t soil_status = greenhouse_soil_status(reading.soil_moisture_percent);
    if (soil_status == GREENHOUSE_SOIL_DRY) {
        return GREENHOUSE_ACTION_WATER;
    }

    return GREENHOUSE_ACTION_IDLE;
}

const char *greenhouse_action_to_string(greenhouse_action_t action)
{
    switch (action) {
    case GREENHOUSE_ACTION_IDLE:
        return "idle";
    case GREENHOUSE_ACTION_WATER:
        return "water";
    case GREENHOUSE_ACTION_ALARM:
        return "alarm";
    default:
        return "unknown";
    }
}

const char *greenhouse_soil_status_to_string(greenhouse_soil_status_t status)
{
    switch (status) {
    case GREENHOUSE_SOIL_INVALID:
        return "invalid";
    case GREENHOUSE_SOIL_DRY:
        return "dry";
    case GREENHOUSE_SOIL_OK:
        return "ok";
    case GREENHOUSE_SOIL_WET:
        return "wet";
    default:
        return "unknown";
    }
}
