
FILE(GLOB bme280_sources src/*.c)
add_library(bme280_lib STATIC ${bme280_sources})
target_include_directories(bme280_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Required to set Zephyr compile options (e.g. for ARM)
target_link_libraries(bme280_lib PUBLIC zephyr_interface)