idf_component_register(SRCS "greenhouse_monitor.c"
                       INCLUDE_DIRS "include"
                       REQUIRES adc_hal adc_moisture_sensor adc_temperature_sensor)
