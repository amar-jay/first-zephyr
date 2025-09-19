#include "./device_tree.c"
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button_cb_data;
static struct gpio_dt_spec led_list[] = {LED0, LED1, LED2, LED3};

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int button_init(void)
{
	int ret;

	if (!device_is_ready(button.port)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return -1;
	}

		ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n",
			       ret, button.port->name, button.pin);
			return 0;
		}

		ret = gpio_pin_interrupt_configure_dt(&button,
						      GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n",
				ret, button.port->name, button.pin);
			return 0;
		}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);
	for (int i = 0; i < ARRAY_SIZE(led_list); i++) {
		struct gpio_dt_spec *led = &led_list[i];
		if (led->port && !gpio_is_ready_dt(led)) {
			printk("Error %d: LED device %s is not ready; ignoring it\n",
			       ret, led->port->name);
			led->port = NULL;
		}

		if (led->port) {
			ret = gpio_pin_configure_dt(led, GPIO_OUTPUT);
			if (ret != 0) {
				printk("Error %d: failed to configure LED device %s pin %d\n",
				       ret, led->port->name, led->pin);
				led->port = NULL;
			} else {
				printk("Set up LED at %s pin %d\n", led->port->name, led->pin);
			}
		}
	}
	for (int i = 0; i < ARRAY_SIZE(led_list); i++) {
		const struct gpio_dt_spec *led = &led_list[i];
		if (!led->port) {
			return -1;
		}
	}
	return 0;
}

int simple_button_toggle(void)
{
	int current_led = 0;
	int ret = button_init();
	if (ret < 0) {
		return 0;
	}

	while (1) {
	 int val = gpio_pin_get_dt(&button);
	 k_msleep(200);
	 if (val > 0) {
	   gpio_pin_toggle_dt(&led_list[current_led]);
	   current_led = (current_led + 1) % ARRAY_SIZE(led_list);
	 }
	}
	return 0;
}