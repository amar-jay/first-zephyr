#include "./device_tree.c"
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>


int threading(void) {
	printk("Hello from threading!\n");
	return 0;
}