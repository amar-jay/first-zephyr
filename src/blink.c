#include <stdio.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include "./device_tree.c"

int init_pin(const struct gpio_dt_spec *led) {
	int ret;

	if (!gpio_is_ready_dt(led)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	}
	return 0;
}

int toggle_pin(const struct gpio_dt_spec *led) {
	int ret;

	ret = gpio_pin_toggle_dt(led);
	if (ret < 0) {
		return -1;
	}
	printf("Toggled pin %d\n", led->pin);
	return 0;
}

int simple_led_blink(void) {
  int ret;
  const struct gpio_dt_spec led_pins[] = {LED0, LED1, LED2, LED3};

  for (int i = 0; i < ARRAY_SIZE(led_pins); i++) {
    ret = init_pin(&led_pins[i]);
    if (ret < 0) {
      return 0;
    }
  }

  while (1) {
		for (int i = 0; i < ARRAY_SIZE(led_pins); i++) {
			ret = toggle_pin(&led_pins[i]);
			if (ret < 0) {
				return 0;
			}
			k_msleep(SLEEP_TIME_MS);
		}
  }
  return 0;
}