#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include "../include/main.h"

LOG_MODULE_REGISTER(esp32, LOG_LEVEL_INF);

// const struct device *bme280_dev = DEVICE_DT_GET(DT_ALIAS(bme280));
// const struct device *ms5607_dev = DEVICE_DT_GET(DT_ALIAS(ms5607));


int main(void)
{
	int rc;


	// if (!device_is_ready(bme280_dev)) {
	// 	LOG_ERR("Device %s is not ready\n", bme280_dev->name);
	// 	return 0;
	// }
	// if (!device_is_ready(ms5607_dev)) {
	// 	LOG_ERR("Device %s is not ready\n", ms5607_dev->name);
	// 	return 0;
	// }

	while (1) {
		// printk("Hello World from Zephyr!\n");
		// LOG_INF("Hello World from Zephyr!");

		/*
		EXAMPLES 
		- blink();
		- button_click();
		- blink_n_click();
		- adc_read_val();
		- random_print();
		- get_gps_info();
		- read_bme280();
		- read_ms5611();
		*/



		k_sleep(K_MSEC(500));
	}

	return 0;
}