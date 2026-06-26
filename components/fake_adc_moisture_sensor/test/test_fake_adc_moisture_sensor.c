#include "fake_adc_moisture_sensor.h"
#include "unity.h"

TEST_CASE("fake adc moisture sensor replays raw values", "[fake_adc_moisture_sensor]")
{
    fake_adc_moisture_sensor_t fake;
    fake_adc_moisture_sensor_init(&fake);
    const int values[] = {100, 250};
    fake_adc_moisture_sensor_set_read_values(&fake, values, 2);
    adc_hal_t hal = fake_adc_moisture_sensor_as_adc_hal(&fake);

    int raw_value = 0;
    TEST_ASSERT_EQUAL(ESP_OK, adc_hal_read_raw(&hal, &raw_value));
    TEST_ASSERT_EQUAL(100, raw_value);
    TEST_ASSERT_EQUAL(ESP_OK, adc_hal_read_raw(&hal, &raw_value));
    TEST_ASSERT_EQUAL(250, raw_value);
}

TEST_CASE("fake adc moisture sensor can return read error", "[fake_adc_moisture_sensor]")
{
    fake_adc_moisture_sensor_t fake;
    fake_adc_moisture_sensor_init(&fake);
    fake_adc_moisture_sensor_set_read_error(&fake, ESP_ERR_TIMEOUT);
    adc_hal_t hal = fake_adc_moisture_sensor_as_adc_hal(&fake);

    int raw_value = 0;
    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, adc_hal_read_raw(&hal, &raw_value));
}
