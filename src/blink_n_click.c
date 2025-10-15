#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// Settings
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(userswitch), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(userled), gpios);

int blink_n_click(void)
{
	// Make sure that the button was initialized
	if (!gpio_is_ready_dt(&btn)) {
		printk("ERROR: button not ready\r\n");
	}
	if (!gpio_is_ready_dt(&led)) {
		printk("ERROR: LED not ready\r\n");
	}
	// Set the button as input (apply extra flags if needed)
	int ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
	if (ret < 0) {
		printk("ERROR: failed to configure button pin\r\n");
	}
	// Set the GPIO as output
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	if (ret < 0) {
		printk("ERROR: failed to configure LED pin\r\n");
	}

	int state = 0;
	// Do forever
	while (1) {
		// Poll button state
		state = gpio_pin_get_dt(&btn);
		printk("Button state: %d\r\n", state);
		// Change the state of the pin and print
		if (state == 0) { // Assuming active low button
			printk("LED OFF\r\n");
			gpio_pin_set_dt(&led, 0);
		} else {
			printk("LED ON\r\n");
			gpio_pin_set_dt(&led, 1);
		}
		// Sleep
		k_sleep(K_MSEC(100));
	}

		return 0;
}