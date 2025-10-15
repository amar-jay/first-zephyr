cmake_minimum_required(VERSION 3.20.0)

# Generate compile_commands.json by default so editors (clangd, cpptools)
# can pick up the build include paths. Override with -DCMAKE_EXPORT_COMPILE_COMMANDS=OFF if you don't want it.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile_commands.json for editor tooling")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(esp32_basic)


# Include the library (via directory)
add_subdirectory(lib/simple_maths)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
# Link the simple_maths library to the application
zephyr_include_directories(config)

target_link_libraries(app PRIVATE maths_lib)