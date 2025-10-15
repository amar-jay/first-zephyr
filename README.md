# esp32-basic

This is a small Zephyr sample for ESP32 boards demonstrating:

- how to use a devicetree overlay to enable and alias devices (GPIO, ADC, PWM)
- basic GPIO output (LED) using DT helpers
- basic ADC read using the ADC API
- basic PWM control using the PWM DT helpers

Files of interest:

- `boards/esp32s3_devkitc.overlay` — example overlay that enables `adc`, `pwm0` and `gpio0` and declares aliases `led0`, `pwm_led0`, and `adc0`.
- `src/main.c` — sample application using `gpio_dt_spec`, `pwm_dt_spec`, and ADC APIs.
- `prj.conf` — minimal configuration for logging, GPIO, ADC and PWM.

Notes and assumptions
- This sample uses generic aliases and example pin numbers. You will likely need to update the overlay to match your exact board (pin numbers, controller nodes and channel IDs).
- On ESP32 variants the ADC and PWM controllers may have different node names. Inspect your board's devicetree (build output or board DTS files) to pick correct controller node names and channels.

Build and flash (example, run from this workspace root):

```bash
# set ZEPHYR_BASE to your Zephyr installation if not already set
west build -b esp32s3_devkitc esp32-basic
west flash
```

If you use a different ESP32 board, replace `-b esp32s3_devkitc` with your board name and update `boards/esp32s3_devkitc.overlay` accordingly.


# To generate compile_commands.json
If VS Code shows 'file not found' for Zephyr headers, regenerate the compilation database and reload the language server.

Regenerate compile commands:

```bash
./scripts/gen-compile-commands.sh
```

Then reload the VS Code window (Developer: Reload Window) or restart clangd.

The workspace contains `.vscode/settings.json` configured to point clangd to the workspace `build` folder where `compile_commands.json` is generated.