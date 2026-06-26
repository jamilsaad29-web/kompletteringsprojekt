idf_component_register(
    SRCS "test_greenhouse_monitor.c"
    REQUIRES greenhouse_monitor fake_adc_moisture_sensor fake_adc_temperature_sensor unity)
