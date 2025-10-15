#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#ifdef CONFIG_MATHS_LIB
#include "maths.h"
#else
#warning "No maths library selected"
#endif

LOG_MODULE_REGISTER(esp32_basic, LOG_LEVEL_INF);

/* Use the standard alias 'userled' (defined in the board overlay). */
#define LED_NODE DT_ALIAS(userled)
#if DT_NODE_HAS_STATUS(LED_NODE, okay)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
#else
#warning "LED PIN alias not defined in devicetree"
static const struct gpio_dt_spec led = {
	.port=0,
	.pin=13,
	.dt_flags=GPIO_ACTIVE_HIGH
};
#endif


int main(void)
{

  LOG_INF("esp32-basic starting");

	int ret;
	int state = 0;

	#ifdef CONFIG_MATHS_LIB
	int sum;
	add(5, 3, &sum);
	LOG_INF("5 + 3 = %d", sum);
	#endif

	// Make sure that the GPIO was initialized
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	// Set the GPIO as output
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	if (ret < 0) {
		return 0;
	}

	// Do forever
	while (1) {

		// Change the state of the pin and print
		LOG_INF("LED state: %d\r\n", state);
		
		// Set pin state
		ret = gpio_pin_set_dt(&led, 0);
		if (ret < 0) {
			return 0;
		}

		// Sleep
		k_sleep(K_MSEC(1000));
	}

	return 0;
}