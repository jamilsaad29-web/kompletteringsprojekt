idf_component_register(
    SRCS "test_adc_temperature_sensor.c"
    REQUIRES adc_temperature_sensor fake_adc_temperature_sensor unity)
