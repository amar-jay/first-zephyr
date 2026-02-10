#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../include/main.h"

LOG_MODULE_REGISTER(esp32, LOG_LEVEL_INF);

// const struct device *bme280_dev = DEVICE_DT_GET(DT_ALIAS(bme280));
// const struct device *ms5607_dev = DEVICE_DT_GET(DT_ALIAS(ms5607));

int main(void) {
  // int rc;

	struct device *mpu9250_dev = DEVICE_DT_GET(DT_ALIAS(mpu9250));
	if (!device_is_ready(mpu9250_dev)) {
		LOG_ERR("MPU9250 Device not ready");
		return 0;
	}
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
		// LOG_INF("Device (Ox75): %x", WHO_AM_I(0x75));
		// LOG_INF("Device (Ox68): %x", WHO_AM_I(0x68));
		// LOG_INF("Device (Ox69): %x", WHO_AM_I(0x69));

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

		int rc = 0;
		struct sensor_value accel[3], gyro[3], mag[3];
		rc = sensor_sample_fetch(mpu9250_dev);
		if (rc < 0) {
			LOG_ERR("MPU9250 sample fetch failed: %d", rc);
		} else {
			sensor_channel_get(mpu9250_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
			sensor_channel_get(mpu9250_dev, SENSOR_CHAN_GYRO_XYZ, gyro);
			sensor_channel_get(mpu9250_dev, SENSOR_CHAN_MAGN_XYZ, mag);
			LOG_INF("MPU9250 Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
					accel[0].val1, accel[0].val2,
					accel[1].val1, accel[1].val2,
					accel[2].val1, accel[2].val2);
			LOG_INF("MPU9250 Gyro: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
					gyro[0].val1, gyro[0].val2,
					gyro[1].val1, gyro[1].val2,
					gyro[2].val1, gyro[2].val2);
			LOG_INF("MPU9250 Mag: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
					mag[0].val1, mag[0].val2,
					mag[1].val1, mag[1].val2,
					mag[2].val1, mag[2].val2);
		}
    k_sleep(K_MSEC(500));
  }

  return 0;
}
