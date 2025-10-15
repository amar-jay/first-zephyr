


# Use bash for recipe execution and enable errexit -u -o pipefail in shell flags
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c

# Compute directories relative to this Makefile. Use make functions so nothing
# is executed at parse time (unlike Bash expansions in the original file).
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
PROJECT_ROOT := $(abspath $(MAKEFILE_DIR))
BUILD_DIR := $(PROJECT_ROOT)/build

.PHONY: build_esp32s3_devkitc build_stm32f4_disco

gen_esp32s3_devkitc:
	cmake -S "$(PROJECT_ROOT)" -B "$(BUILD_DIR)" -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay -DBOARD=esp32s3_devkitc/esp32s3/procpu
	echo "compile_commands.json regenerated at $(BUILD_DIR)/compile_commands.json"
	west build -b esp32s3_devkitc/esp32s3/procpu --pristine -- -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay

gen_stm32f4_disco:
	cmake -S "$(PROJECT_ROOT)" -B "$(BUILD_DIR)" -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DDTC_OVERLAY_FILE=boards/stm32f4_disco.overlay -DBOARD=stm32f4_disco
	echo "compile_commands.json regenerated at $(BUILD_DIR)/compile_commands.json"
	west build -b stm32f4_disco --pristine -- -DDTC_OVERLAY_FILE=boards/stm32f4_disco.overlay
