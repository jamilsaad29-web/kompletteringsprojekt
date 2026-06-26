#include "adc_temperature_sensor.h"
#include "fake_adc_temperature_sensor.h"
#include "unity.h"

TEST_CASE("adc temperature sensor converts raw value to celsius", "[adc_temperature_sensor]")
{
    int temperature_c = 0;

    TEST_ASSERT_EQUAL(ESP_OK, adc_temperature_sensor_raw_to_celsius(0, &temperature_c));
    TEST_ASSERT_EQUAL(-40, temperature_c);

    TEST_ASSERT_EQUAL(ESP_OK, adc_temperature_sensor_raw_to_celsius(2047, &temperature_c));
    TEST_ASSERT_EQUAL(22, temperature_c);

    TEST_ASSERT_EQUAL(ESP_OK, adc_temperature_sensor_raw_to_celsius(4095, &temperature_c));
    TEST_ASSERT_EQUAL(85, temperature_c);
}

TEST_CASE("adc temperature sensor reads celsius through adc hal", "[adc_temperature_sensor]")
{
    fake_adc_temperature_sensor_t fake;
    fake_adc_temperature_sensor_init(&fake);
    const int values[] = {2079};
    fake_adc_temperature_sensor_set_read_values(&fake, values, 1);
    adc_hal_t hal = fake_adc_temperature_sensor_as_adc_hal(&fake);

    int temperature_c = 0;
    TEST_ASSERT_EQUAL(ESP_OK, adc_temperature_sensor_read_celsius(&hal, &temperature_c));

    TEST_ASSERT_EQUAL(23, temperature_c);
    TEST_ASSERT_EQUAL(1, fake.read_count);
}

TEST_CASE("adc temperature sensor propagates adc errors", "[adc_temperature_sensor]")
{
    fake_adc_temperature_sensor_t fake;
    fake_adc_temperature_sensor_init(&fake);
    fake_adc_temperature_sensor_set_read_error(&fake, ESP_ERR_TIMEOUT);
    adc_hal_t hal = fake_adc_temperature_sensor_as_adc_hal(&fake);

    int temperature_c = 0;
    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, adc_temperature_sensor_read_celsius(&hal, &temperature_c));
}
