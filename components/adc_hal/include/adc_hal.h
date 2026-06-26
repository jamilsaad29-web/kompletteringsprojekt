#pragma once

#include "esp_err.h"

typedef struct adc_hal {
    void *ctx;
    esp_err_t (*read_raw)(void *ctx, int *raw_value);
} adc_hal_t;

static inline esp_err_t adc_hal_read_raw(const adc_hal_t *hal, int *raw_value)
{
    if (hal == NULL || hal->read_raw == NULL || raw_value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    return hal->read_raw(hal->ctx, raw_value);
}
