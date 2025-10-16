#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#include "../include/main.h"




int main(void)
{
	// blink(&led);
	// button_click();
	// blink_n_click();
	adc_read_val();
	//  random_print();
	return 0;
}