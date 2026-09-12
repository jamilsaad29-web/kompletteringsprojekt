#include "fake_adc_moisture_sensor.h"
#include "fake_adc_temperature_sensor.h"
#include "greenhouse_monitor.h"
#include "unity.h"

TEST_CASE("soil status classifies dry ok and wet values", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_DRY, greenhouse_soil_status(0));
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_DRY, greenhouse_soil_status(34));
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_OK, greenhouse_soil_status(35));
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_OK, greenhouse_soil_status(70));
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_WET, greenhouse_soil_status(71));
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_WET, greenhouse_soil_status(100));
}

TEST_CASE("soil status rejects impossible moisture values", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_INVALID, greenhouse_soil_status(-1));
    TEST_ASSERT_EQUAL(GREENHOUSE_SOIL_INVALID, greenhouse_soil_status(101));
}

TEST_CASE("temperature status classifies low and normal temperatures", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL(GREENHOUSE_TEMPERATURE_LOW,
                      greenhouse_temperature_status(10));

    TEST_ASSERT_EQUAL(GREENHOUSE_TEMPERATURE_OK,
                      greenhouse_temperature_status(22));
}

TEST_CASE("temperature status classifies high and critical temperatures", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL(GREENHOUSE_TEMPERATURE_HIGH,
                      greenhouse_temperature_status(28));

    TEST_ASSERT_EQUAL(GREENHOUSE_TEMPERATURE_CRITICAL,
                      greenhouse_temperature_status(35));
}

TEST_CASE("temperature status rejects invalid temperatures", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL(GREENHOUSE_TEMPERATURE_INVALID,
                      greenhouse_temperature_status(-41));

    TEST_ASSERT_EQUAL(GREENHOUSE_TEMPERATURE_INVALID,
                      greenhouse_temperature_status(86));
}

TEST_CASE("should water returns true for dry soil", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 22,
        .soil_moisture_percent = 20,
    };

    TEST_ASSERT_TRUE(greenhouse_should_water(reading));
}

TEST_CASE("should water returns false when soil is not dry", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 22,
        .soil_moisture_percent = 50,
    };

    TEST_ASSERT_FALSE(greenhouse_should_water(reading));
}

TEST_CASE("should water returns false for invalid reading", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 22,
        .soil_moisture_percent = -1,
    };

    TEST_ASSERT_FALSE(greenhouse_should_water(reading));
}

TEST_CASE("reading validation accepts realistic sensor readings", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 24,
        .soil_moisture_percent = 55,
    };

    TEST_ASSERT_TRUE(greenhouse_reading_is_valid(reading));
}

TEST_CASE("reading validation rejects impossible sensor readings", "[greenhouse_monitor]")
{
    greenhouse_reading_t invalid_raw = {
        .temperature_c = -41,
        .soil_moisture_percent = 55,
    };
    greenhouse_reading_t too_wet = {
        .temperature_c = 24,
        .soil_moisture_percent = 101,
    };

    TEST_ASSERT_FALSE(greenhouse_reading_is_valid(invalid_raw));
    TEST_ASSERT_FALSE(greenhouse_reading_is_valid(too_wet));
}

TEST_CASE("action is idle when climate is healthy", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 23,
        .soil_moisture_percent = 50,
    };

    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_IDLE, greenhouse_decide_action(reading));
}

TEST_CASE("action waters dry soil", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 22,
        .soil_moisture_percent = 20,
    };

    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_WATER, greenhouse_decide_action(reading));
}

TEST_CASE("action raises alarm when temperature is critical", "[greenhouse_monitor]")
{
    greenhouse_reading_t reading = {
        .temperature_c = 35,
        .soil_moisture_percent = 55,
    };

    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_ALARM, greenhouse_decide_action(reading));
}

TEST_CASE("action raises alarm for invalid readings", "[greenhouse_monitor]")
{
    greenhouse_reading_t invalid_soil = {
        .temperature_c = 24,
        .soil_moisture_percent = -1,
    };

    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_ALARM, greenhouse_decide_action(invalid_soil));
}

TEST_CASE("monitor update reads adc sensors and decides action", "[greenhouse_monitor]")
{
    fake_adc_moisture_sensor_t moisture_fake;
    fake_adc_temperature_sensor_t temperature_fake;
    fake_adc_moisture_sensor_init(&moisture_fake);
    fake_adc_temperature_sensor_init(&temperature_fake);

    const int moisture_values[] = {819};
    const int temperature_values[] = {2079};

    fake_adc_moisture_sensor_set_read_values(&moisture_fake, moisture_values, 1);
    fake_adc_temperature_sensor_set_read_values(&temperature_fake, temperature_values, 1);

    adc_hal_t moisture_hal = fake_adc_moisture_sensor_as_adc_hal(&moisture_fake);
    adc_hal_t temperature_hal = fake_adc_temperature_sensor_as_adc_hal(&temperature_fake);

    greenhouse_monitor_result_t result = greenhouse_monitor_update(&moisture_hal, &temperature_hal);

    TEST_ASSERT_EQUAL(ESP_OK, result.read_error);
    TEST_ASSERT_EQUAL(1, moisture_fake.read_count);
    TEST_ASSERT_EQUAL(1, temperature_fake.read_count);
    TEST_ASSERT_EQUAL(23, result.reading.temperature_c);
    TEST_ASSERT_EQUAL(20, result.reading.soil_moisture_percent);
    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_WATER, result.action);
}

TEST_CASE("monitor update raises alarm when moisture adc read fails", "[greenhouse_monitor]")
{
    fake_adc_moisture_sensor_t moisture_fake;
    fake_adc_temperature_sensor_t temperature_fake;
    fake_adc_moisture_sensor_init(&moisture_fake);
    fake_adc_temperature_sensor_init(&temperature_fake);

    fake_adc_moisture_sensor_set_read_error(&moisture_fake, ESP_ERR_TIMEOUT);

    adc_hal_t moisture_hal = fake_adc_moisture_sensor_as_adc_hal(&moisture_fake);
    adc_hal_t temperature_hal = fake_adc_temperature_sensor_as_adc_hal(&temperature_fake);

    greenhouse_monitor_result_t result = greenhouse_monitor_update(&moisture_hal, &temperature_hal);

    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, result.read_error);
    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_ALARM, result.action);
}

TEST_CASE("monitor update raises alarm when temperature adc read fails", "[greenhouse_monitor]")
{
    fake_adc_moisture_sensor_t moisture_fake;
    fake_adc_temperature_sensor_t temperature_fake;
    fake_adc_moisture_sensor_init(&moisture_fake);
    fake_adc_temperature_sensor_init(&temperature_fake);

    const int moisture_values[] = {2047};

    fake_adc_moisture_sensor_set_read_values(&moisture_fake, moisture_values, 1);
    fake_adc_temperature_sensor_set_read_error(&temperature_fake, ESP_ERR_TIMEOUT);

    adc_hal_t moisture_hal = fake_adc_moisture_sensor_as_adc_hal(&moisture_fake);
    adc_hal_t temperature_hal = fake_adc_temperature_sensor_as_adc_hal(&temperature_fake);

    greenhouse_monitor_result_t result = greenhouse_monitor_update(&moisture_hal, &temperature_hal);

    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, result.read_error);
    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_ALARM, result.action);
}

TEST_CASE("monitor update raises alarm when adc hal is missing", "[greenhouse_monitor]")
{
    greenhouse_monitor_result_t result = greenhouse_monitor_update(NULL, NULL);

    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, result.read_error);
    TEST_ASSERT_EQUAL(GREENHOUSE_ACTION_ALARM, result.action);
}

TEST_CASE("action to string returns stable labels", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL_STRING("idle", greenhouse_action_to_string(GREENHOUSE_ACTION_IDLE));
    TEST_ASSERT_EQUAL_STRING("water", greenhouse_action_to_string(GREENHOUSE_ACTION_WATER));
    TEST_ASSERT_EQUAL_STRING("alarm", greenhouse_action_to_string(GREENHOUSE_ACTION_ALARM));
    TEST_ASSERT_EQUAL_STRING("unknown", greenhouse_action_to_string((greenhouse_action_t)99));
}

TEST_CASE("soil status to string returns stable labels", "[greenhouse_monitor]")
{
    TEST_ASSERT_EQUAL_STRING("invalid", greenhouse_soil_status_to_string(GREENHOUSE_SOIL_INVALID));
    TEST_ASSERT_EQUAL_STRING("dry", greenhouse_soil_status_to_string(GREENHOUSE_SOIL_DRY));
    TEST_ASSERT_EQUAL_STRING("ok", greenhouse_soil_status_to_string(GREENHOUSE_SOIL_OK));
    TEST_ASSERT_EQUAL_STRING("wet", greenhouse_soil_status_to_string(GREENHOUSE_SOIL_WET));
    TEST_ASSERT_EQUAL_STRING("unknown", greenhouse_soil_status_to_string((greenhouse_soil_status_t)99));
}
