cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile_commands.json for editor tooling")

set(BOARD stm32f4_disco)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(another_zephyr)


FILE(GLOB app_sources src/*.c)
FILE(GLOB app_headers ./include/*.h)
target_sources(app PRIVATE ${app_sources} ${app_headers})

zephyr_include_directories(config ${CMAKE_CURRENT_SOURCE_DIR}/config)






