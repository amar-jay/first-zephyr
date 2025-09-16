#include <stdio.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led_0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led_3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

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
  const struct gpio_dt_spec *led_pins[] = {&led_0, &led_1, &led_2, &led_3};

  for (int i = 0; i < ARRAY_SIZE(led_pins); i++) {
    ret = init_pin(led_pins[i]);
    if (ret < 0) {
      return 0;
    }
  }

  while (1) {
		for (int i = 0; i < ARRAY_SIZE(led_pins); i++) {
			ret = toggle_pin(led_pins[i]);
			if (ret < 0) {
				return 0;
			}
			k_msleep(SLEEP_TIME_MS);
		}
  }
  return 0;
}