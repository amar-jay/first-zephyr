#ifndef MAIN_H
#define MAIN_H
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

int simple_adc_read(void);
int simple_blink(void);
int simple_button_toggle(void);
int simple_lcd(void);
int random_print(void);
#endif // MAIN_H