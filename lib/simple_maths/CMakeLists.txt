
FILE(GLOB maths_sources src/*.c)
add_library(maths_lib STATIC ${maths_sources})
target_include_directories(maths_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Required to set Zephyr compile options (e.g. for ARM)
target_link_libraries(maths_lib PUBLIC zephyr_interface)