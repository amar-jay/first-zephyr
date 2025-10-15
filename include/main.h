#ifndef MAIN_H
#define MAIN_H
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

int random_print(void);
int blink();
int button_click(void);
int blink_n_click(void);

#endif // MAIN_H