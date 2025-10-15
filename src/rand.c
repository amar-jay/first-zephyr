#include <zephyr/random/random.h>

int random_print(void)
{ 

	uint8_t rand;
	double rand_double;

	while (true) {
		rand = sys_rand8_get();
		rand_double = (double)rand / (UINT8_MAX + 1);
		printk("Random 8-bit integer: %u, Random double: %f\n", rand, rand_double);
		k_sleep(K_MSEC(1000));
	}

	return 0;
}