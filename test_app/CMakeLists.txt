cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS ../components)

# Include unit tests from this component's test subdirectory.
set(TEST_COMPONENTS "greenhouse_monitor;adc_hal;adc_moisture_sensor;adc_temperature_sensor;fake_adc_moisture_sensor;fake_adc_temperature_sensor" CACHE STRING "List of components to test" FORCE)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
idf_build_set_property(MINIMAL_BUILD ON)
project(example_test_app)
