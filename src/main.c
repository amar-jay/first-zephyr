#include <stdio.h>
#include "./blink.c"
#include "./button.c"
#include "./device_tree.c"

#include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/pwm.h>


int main(void) {
	printf("Hello World! %s\n", CONFIG_BOARD);
	// simple_button_toggle();
	simple_led_blink();
	return 0;
}