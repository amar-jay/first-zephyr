#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

/*
	* The devicetree node identifier for the "led1" alias.
	* Change "led1" to "led2", "led3", etc to get access
*/
#define LED0 GPIO_DT_SPEC_GET(LED0_NODE, gpios)
#define LED1 GPIO_DT_SPEC_GET(LED1_NODE, gpios)
#define LED2 GPIO_DT_SPEC_GET(LED2_NODE, gpios)
#define LED3 GPIO_DT_SPEC_GET(LED3_NODE, gpios)

// #define PWM_LED0 PWM_DT_SPEC_GET(LED0_NODE)
#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif