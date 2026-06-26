#include "adc_moisture_sensor.h"
#include "fake_adc_moisture_sensor.h"
#include "unity.h"

TEST_CASE("adc moisture sensor converts raw value to percent", "[adc_moisture_sensor]")
{
    int percent = 0;

    TEST_ASSERT_EQUAL(ESP_OK, adc_moisture_sensor_raw_to_percent(0, &percent));
    TEST_ASSERT_EQUAL(0, percent);

    TEST_ASSERT_EQUAL(ESP_OK, adc_moisture_sensor_raw_to_percent(2047, &percent));
    TEST_ASSERT_EQUAL(49, percent);

    TEST_ASSERT_EQUAL(ESP_OK, adc_moisture_sensor_raw_to_percent(4095, &percent));
    TEST_ASSERT_EQUAL(100, percent);
}

TEST_CASE("adc moisture sensor reads percent through adc hal", "[adc_moisture_sensor]")
{
    fake_adc_moisture_sensor_t fake;
    fake_adc_moisture_sensor_init(&fake);
    const int values[] = {819};
    fake_adc_moisture_sensor_set_read_values(&fake, values, 1);
    adc_hal_t hal = fake_adc_moisture_sensor_as_adc_hal(&fake);

    int percent = 0;
    TEST_ASSERT_EQUAL(ESP_OK, adc_moisture_sensor_read_percent(&hal, &percent));

    TEST_ASSERT_EQUAL(20, percent);
    TEST_ASSERT_EQUAL(1, fake.read_count);
}

TEST_CASE("adc moisture sensor propagates adc errors", "[adc_moisture_sensor]")
{
    fake_adc_moisture_sensor_t fake;
    fake_adc_moisture_sensor_init(&fake);
    fake_adc_moisture_sensor_set_read_error(&fake, ESP_ERR_TIMEOUT);
    adc_hal_t hal = fake_adc_moisture_sensor_as_adc_hal(&fake);

    int percent = 0;
    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, adc_moisture_sensor_read_percent(&hal, &percent));
}
