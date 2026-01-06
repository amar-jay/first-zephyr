#ifdef MS5607
#include <zephyr/drivers/sensor.h>

void read_ms5611(struct ms5611_dev *ms5611_dev){
		int rc;
		struct sensor_value temp2 , pres;
		

		struct sensor_value temp, press;

		rc = sensor_sample_fetch(ms5607_dev);
		if (rc == 0) {
			rc = sensor_channel_get(ms5607_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
			if (rc == 0) {
				LOG_INF("Temperature: %d.%06d C", temp.val1, temp.val2);
			} else {
				LOG_ERR("Could not get temperature: %d", rc);
			}

			rc = sensor_channel_get(ms5607_dev, SENSOR_CHAN_PRESS, &press);
			if (rc == 0) {
				LOG_INF("Pressure: %d.%06d kPa", press.val1, press.val2);
			} else {
				LOG_ERR("Could not get pressure: %d", rc);
			}
			
		} else {
			LOG_ERR("Sensor sample fetch failed: %d", rc);
		}
	}
#endif // MS5607