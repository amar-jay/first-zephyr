#include <zephyr/logging/log.h>
#include <stdio.h>
#include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/devicetree.h>
// #include <zephyr/drivers/gpio.h>

// #include "../include/main.h"



LOG_MODULE_REGISTER(esp32, LOG_LEVEL_INF);

int main(void)
{
	while (1) {
		printf("Hello World from Zephyr!\n");
		printk("Hello World from Zephyr!\n");
		LOG_INF("Hello World from Zephyr!");
		k_sleep(K_MSEC(500));
	}
	// blink();
	// button_click();
	// blink_n_click();
	// adc_read_val();
	//  random_print();
	return 0;
}