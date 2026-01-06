#ifdef BME280
#include <zephyr/drivers/sensor.h>

double intial_pressure = 0;
double initial_altitude = 0;

double altitude_m(double pressure_pa)
{
    return 44330.0 * (1.0 - pow(pressure_pa / 101325, 0.1903));
}

void read_bme280(struct bme280_dev *bme280_dev){
		int rc;
		struct sensor_value temp2 , pres;
		// Sleep for 500 milliseconds
		// k_sleep(K_MSEC(500));

		rc = sensor_sample_fetch(bme280_dev);
		if (rc == 0) {
			rc = sensor_channel_get(bme280_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp2);
			if (rc == 0) {
				LOG_INF("BME280 Temperature: %d.%06d C", temp2.val1, temp2.val2);
			} else {
				LOG_ERR("Could not get BME280 temperature: %d", rc);
			}
		} else {
			LOG_ERR("BME280 Sensor sample fetch failed: %d", rc);
		}


		double current_pressure;
		double current_altitude;
		if (rc == 0) {
			rc = sensor_channel_get(bme280_dev, SENSOR_CHAN_PRESS, &pres);
			if (rc == 0) {
				if (*initial_pressure == 0) {
					*initial_pressure = pres.val1 + pres.val2 / 1000000.0;
					*initial_altitude = altitude_m(*initial_pressure * 1000); // convert kPa to Pa
					LOG_INF("Initial Pressure Reading Set: %.6f kPa", *initial_pressure);
				} else {
					current_pressure = pres.val1 + pres.val2 / 1000000.0;
					current_altitude = altitude_m(current_pressure * 1000); // convert kPa to Pa
					LOG_INF("Pressure Change: %.6f kPa", (*initial_pressure - current_pressure) * 1000 );
					LOG_INF("Altitude Change: %.6f meters", current_altitude - *initial_altitude);
					
					LOG_INF("BME280 Elevation: %.2f meters", current_altitude);
					LOG_INF("BME280 Pressure: %.6f kPa", current_pressure);
					LOG_INF("---------------------------------");
				}
			} else {
				LOG_ERR("Could not get BME280 pressure: %d", rc);
			}
		} else {
			LOG_ERR("BME280 Sensor sample fetch failed: %d", rc);
		}
	}
#endif // BME280