#include "adc_hal.h"
#include "unity.h"

static esp_err_t stub_read_ok(void *ctx, int *raw_value)
{
    if (raw_value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    int *captured_reads = (int *)ctx;
    if (captured_reads != NULL) {
        (*captured_reads)++;
    }

    *raw_value = 1234;
    return ESP_OK;
}

TEST_CASE("adc hal rejects invalid arguments", "[adc_hal]")
{
    adc_hal_t hal = {0};
    int raw_value = 0;

    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, adc_hal_read_raw(NULL, &raw_value));
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, adc_hal_read_raw(&hal, &raw_value));

    hal.read_raw = stub_read_ok;
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, adc_hal_read_raw(&hal, NULL));
}

TEST_CASE("adc hal delegates read to implementation", "[adc_hal]")
{
    int read_count = 0;
    adc_hal_t hal = {
        .ctx = &read_count,
        .read_raw = stub_read_ok,
    };
    int raw_value = 0;

    TEST_ASSERT_EQUAL(ESP_OK, adc_hal_read_raw(&hal, &raw_value));

    TEST_ASSERT_EQUAL(1234, raw_value);
    TEST_ASSERT_EQUAL(1, read_count);
}
