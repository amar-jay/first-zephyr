#ifndef MAIN_H
#define MAIN_H
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gnss.h>

int random_print(void);
int blink();
int button_click(void);
int blink_n_click(void);
int adc_read_val(void);
int get_gps_info(void);

#endif // MAIN_H