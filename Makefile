# Use bash for recipe execution with safe shell options
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c

# Core directories
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
PROJECT_ROOT := $(abspath $(MAKEFILE_DIR))
BUILD_DIR := $(PROJECT_ROOT)/build
PWD := $(shell pwd)

# Variables commonly used across builds
ZEPHYR_COMMON_FLAGS := -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -GNinja

# Board-specific configurations
ESP32S3_FLAGS := -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay \
                 -DCONF_FILE="$(PWD)/prj.conf;$(PWD)/boards/esp32s3_devkitc.conf" \
                 -DBOARD=esp32s3_devkitc/esp32s3/procpu

STM32F4_FLAGS := -DDTC_OVERLAY_FILE=boards/stm32f4_disco.overlay \
                 -DCONF_FILE="$(PWD)/prj.conf;$(PWD)/boards/stm32f4_disco.conf" \
                 -DBOARD=stm32f4_disco

.PHONY: all gen_esp32s3_devkitc gen_stm32f4_disco clean

all: gen_esp32s3_devkitc gen_stm32f4_disco

gen_esp32s3_devkitc:
	@echo "[ESP32S3] Configuring build with Zephyr..."
	cmake -S "$(PROJECT_ROOT)" -B "$(BUILD_DIR)" $(ZEPHYR_COMMON_FLAGS) $(ESP32S3_FLAGS)
	@echo "compile_commands.json regenerated at $(BUILD_DIR)/compile_commands.json"
	west build -b esp32s3_devkitc/esp32s3/procpu --pristine -- $(ESP32S3_FLAGS)

gen_stm32f4_disco:
	@echo "[STM32F4] Configuring build with Zephyr..."
	cmake -S "$(PROJECT_ROOT)" -B "$(BUILD_DIR)" $(ZEPHYR_COMMON_FLAGS) $(STM32F4_FLAGS)
	@echo "compile_commands.json regenerated at $(BUILD_DIR)/compile_commands.json"
	west build -b stm32f4_disco --pristine -- $(STM32F4_FLAGS)

clean:
	@echo "🧹 Cleaning build directory..."
	rm -rf "$(BUILD_DIR)"
