#!/usr/bin/env bash
# Regenerate compile_commands.json for this Zephyr app
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}/.."
BUILD_DIR="${PROJECT_ROOT}/build"
cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay
echo "compile_commands.json regenerated at ${BUILD_DIR}/compile_commands.json"
