idf_component_register(
    SRCS "test_adc_moisture_sensor.c"
    REQUIRES adc_moisture_sensor fake_adc_moisture_sensor unity)
