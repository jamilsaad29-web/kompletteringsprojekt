#include "fake_adc_temperature_sensor.h"
#include "unity.h"

TEST_CASE("fake adc temperature sensor replays raw values", "[fake_adc_temperature_sensor]")
{
    fake_adc_temperature_sensor_t fake;
    fake_adc_temperature_sensor_init(&fake);
    const int values[] = {1000, 2000};
    fake_adc_temperature_sensor_set_read_values(&fake, values, 2);
    adc_hal_t hal = fake_adc_temperature_sensor_as_adc_hal(&fake);

    int raw_value = 0;
    TEST_ASSERT_EQUAL(ESP_OK, adc_hal_read_raw(&hal, &raw_value));
    TEST_ASSERT_EQUAL(1000, raw_value);
    TEST_ASSERT_EQUAL(ESP_OK, adc_hal_read_raw(&hal, &raw_value));
    TEST_ASSERT_EQUAL(2000, raw_value);
}

TEST_CASE("fake adc temperature sensor can return read error", "[fake_adc_temperature_sensor]")
{
    fake_adc_temperature_sensor_t fake;
    fake_adc_temperature_sensor_init(&fake);
    fake_adc_temperature_sensor_set_read_error(&fake, ESP_ERR_TIMEOUT);
    adc_hal_t hal = fake_adc_temperature_sensor_as_adc_hal(&fake);

    int raw_value = 0;
    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, adc_hal_read_raw(&hal, &raw_value));
}
