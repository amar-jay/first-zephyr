#include <stdint.h>
#include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
// #include <errno.h>
#include "bme280.h" 

#include "./calibration.c"

#define BME280_REG_ID             0xD0
#define BME280_REG_RESET          0xE0
#define BME280_REG_CTRL_HUM       0xF2
#define BME280_REG_STATUS         0xF3
#define BME280_REG_CTRL_MEAS      0xF4
#define BME280_REG_CONFIG         0xF5
#define BME280_REG_PRESSURE        0xF7
#define BME280_REG_TEMPERATURE     0xFA
#define BME280_REG_HUMIDITY       0xF2
#define BME280_REG_TEMP_PRESS_CALIB_DATA          UINT8_C(0x88)
#define BME280_REG_HUMIDITY_CALIB_DATA            UINT8_C(0xE1)


#define BME280_CHIP_ID            0x60
#define BME280_RESET_CMD          0xB6

#define BME280_I2C_ADDR_PRIMARY   0x76
#define BME280_I2C_ADDR_SECONDARY 0x77

#define BME280_OVERSAMPLING_X1     0x01 // using this for default settings


int bme280_get_calib_data(struct bme280_dev *dev);

int _verify_i2c_address(struct bme280_dev *dev, uint8_t i2c_addr) {
	uint8_t chip_id;
	if (i2c_addr != 0U) {
		if (i2c_addr != BME280_I2C_ADDR_PRIMARY &&
		    i2c_addr != BME280_I2C_ADDR_SECONDARY) {
			return -EINVAL;
		}

		if (i2c_reg_read_byte(dev->i2c_dev, i2c_addr, BME280_REG_ID, &chip_id) != 0 ||
		    chip_id != BME280_CHIP_ID) {
			return -EIO;
		}

		dev->i2c_addr = i2c_addr;
		return 0;
	}

	if (i2c_reg_read_byte(dev->i2c_dev, BME280_I2C_ADDR_PRIMARY, BME280_REG_ID, &chip_id) == 0 &&
	    chip_id == BME280_CHIP_ID) {
		dev->i2c_addr = BME280_I2C_ADDR_PRIMARY;
		return 0;
	}

	if (i2c_reg_read_byte(dev->i2c_dev, BME280_I2C_ADDR_SECONDARY, BME280_REG_ID, &chip_id) == 0 &&
	    chip_id == BME280_CHIP_ID) {
		dev->i2c_addr = BME280_I2C_ADDR_SECONDARY;
		return 0;
	}

	return -EIO;
}

int bme280_init(struct bme280_dev *dev, const struct device *i2c_dev, uint8_t i2c_addr)
{
	if (!device_is_ready(i2c_dev)) {
		return -ENODEV;
	}
	int ret;

	dev->i2c_dev = i2c_dev;
	// auto-detect I2C address if not predefined
	ret = _verify_i2c_address(dev, i2c_addr);
	if (ret < 0) {
		return ret;
	}

  // Reset the device
  ret = i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_RESET, BME280_RESET_CMD);
  if (ret < 0) {
      return -EIO;
  }

	k_sleep(K_MSEC(100));

	// set the ctrl_meas register to default (temp and pressure oversampling x1, mode normal)
	uint8_t ctrl_meas = (BME280_OVERSAMPLING_X1 << 5) | (BME280_OVERSAMPLING_X1 << 2) | 0x03;
	ret = i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, ctrl_meas);
	if (ret < 0) {
		return -EIO;
	}
	// set the ctrl_hum register to default (humidity oversampling x1)
	uint8_t ctrl_hum = BME280_OVERSAMPLING_X1;
	ret = i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_HUM, ctrl_hum);
	if (ret < 0) {
		return -EIO;
	}

  // Standby time and filter config 
	uint8_t config = (0x03 << 5) | (0x00 << 2); // standby 250ms, filter off
	ret = i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CONFIG, config);
	if (ret < 0) {
		return -EIO;
	}
	// Read calibration data
	ret = bme280_get_calib_data(dev);
	if (ret < 0) {
		return -EIO;
	}

	return 0;
}

int bme280_set_mode(struct bme280_dev *dev, uint8_t mode) {
	uint8_t ctrl_meas;

	// Read the current CTRL_MEAS register value
	if (i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, &ctrl_meas) < 0) {
			return -EIO;
	}

	// Clear the mode bits (bits 1:0)
	ctrl_meas &= ~0x03;

	// Set the new mode value
	ctrl_meas |= (mode & 0x03);

	// Write back the updated CTRL_MEAS register value
	if (i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, ctrl_meas) < 0) {
		return -EIO;
	}
	return 0;
}


int bme280_set_temp_sampling(struct bme280_dev *dev, bme280_oversampling_t osrs_t) {
	uint8_t ctrl_meas;

	// Read the current CTRL_MEAS register value
	if (i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, &ctrl_meas) < 0) {
			return -EIO;
	}

	// Clear the temperature oversampling bits (bits 7:5)
	ctrl_meas &= ~0xE0;

	// Set the new temperature oversampling value
	ctrl_meas |= (osrs_t << 5);

	// Write back the updated CTRL_MEAS register value
	if (i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, ctrl_meas) < 0) {
		return -EIO;
	}
	return 0;
}

int bme280_set_pressure_sampling(struct bme280_dev *dev, bme280_oversampling_t osrs_p) {
	uint8_t ctrl_meas;

	// Read the current CTRL_MEAS register value
	if (i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, &ctrl_meas) < 0) {
			return -EIO;
	}

	// Clear the pressure oversampling bits (bits 4:2)
	ctrl_meas &= ~0x1C;

	// Set the new pressure oversampling value
	ctrl_meas |= (osrs_p << 2);

	// Write back the updated CTRL_MEAS register value
	if (i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, ctrl_meas) < 0) {
		return -EIO;
	}
	return 0;
}

int bme280_set_humidity_sampling(struct bme280_dev *dev, bme280_oversampling_t osrs_h) {
	uint8_t ctrl_hum;
	uint8_t ctrl_meas;

	// Read the current CTRL_HUM register value
	if (i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_HUM, &ctrl_hum) < 0) {
			return -EIO;
	}

	// to write to CTRL_HUM register first, as per datasheet
	// "Changes to this register only become effective after a write operation to CTRL_MEAS register."
	if (i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, &ctrl_meas) < 0) {
			return -EIO;
	}

	// validate the ctrl_meas is not in sleep mode
	if ((ctrl_meas & 0x03) == BME280_SLEEP_MODE) {
		return -EINVAL;
	}

	if (i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_MEAS, ctrl_meas) < 0) {
			return -EIO;
	}

	// Clear the humidity oversampling bits (bits 2:0)
	ctrl_hum &= ~0x07;

	// Set the new humidity oversampling value
	ctrl_hum |= (osrs_h & 0x07);

	// Write back the updated CTRL_HUM register value
	if (i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CTRL_HUM, ctrl_hum) < 0) {
		return -EIO;
	}

	return 0;
}

int bme280_configure(struct bme280_dev *dev, bme280_standby_time_t standby_time, bme280_filter_coefficient_t filter_coefficient) {
	uint8_t config;

	// Configure the CONFIG register
	config = (standby_time << 5) | (filter_coefficient << 2);
	if (i2c_reg_write_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_CONFIG, config) < 0) {
		return -EIO;
	}

	return 0;
}

#ifdef BME280_DOUBLE_ENABLE
double bme280_read_temp(struct bme280_dev *dev, int32_t *temp)
#else
uint32_t bme280_read_temp(struct bme280_dev *dev, int32_t *temp)
#endif
{
	uint8_t uncomp_data[3];

	if (temp == NULL) {
		return -EINVAL;
	}

	int ret = i2c_burst_read(dev->i2c_dev, dev->i2c_addr, BME280_REG_TEMPERATURE, uncomp_data, 3);
	if (ret < 0) {
		return -EIO;
	}

	*temp = compensate_temperature((const uint32_t *)uncomp_data, &dev->calib);

	return 0;
}

#ifdef BME280_DOUBLE_ENABLE
double bme280_read_pressure(struct bme280_dev *dev, int32_t *pressure)
#else
uint32_t bme280_read_pressure(struct bme280_dev *dev, int32_t *pressure)
#endif
{
	uint8_t uncomp_data[3] = {0};

	if (pressure == NULL) {
		return -EINVAL;
	}

	int ret = i2c_burst_read(dev->i2c_dev, dev->i2c_addr, BME280_REG_PRESSURE, uncomp_data, 3);
	if (ret < 0) {
		return -EIO;
	}

	*pressure = compensate_pressure((const uint32_t *)uncomp_data, &dev->calib);

	return 0;
}

#ifdef BME280_DOUBLE_ENABLE
double bme280_read_humidity(struct bme280_dev *dev, int32_t *humidity)
#else
uint32_t bme280_read_humidity(struct bme280_dev *dev, int32_t *humidity)
#endif
{
	uint8_t uncomp_data[2] = {0};

	if (humidity == NULL) {
		return -EINVAL;
	}

	int ret = i2c_burst_read(dev->i2c_dev, dev->i2c_addr, BME280_REG_HUMIDITY, uncomp_data, 2);
	if (ret < 0) {
		return -EIO;
	}

	*humidity = compensate_humidity((const uint32_t *)uncomp_data, &dev->calib);

	return 0;
}

/*!
 * @brief This internal API reads the calibration data from the sensor, parse
 * it and store in the device structure.
 */
int bme280_get_calib_data(struct bme280_dev *dev)
{
    int8_t rslt;

    /* Array to store calibration data */
    uint8_t calib_data[26] = { 0 };

    /* Read the calibration data from the sensor */
		rslt = i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_TEMP_PRESS_CALIB_DATA, calib_data);
		if (rslt < 0) {
				return -EIO;
		}

    if (rslt == 0)
    {
        /* Parse temperature and pressure calibration data and store
         * it in device structure
         */
        parse_temp_press_calib_data(calib_data, dev);

        /* Read the humidity calibration data from the sensor */
        rslt = i2c_reg_read_byte(dev->i2c_dev, dev->i2c_addr, BME280_REG_HUMIDITY_CALIB_DATA, calib_data);
        if (rslt < 0) {
            return -EIO;
        }

        if (rslt == 0)
        {
            /* Parse humidity calibration data and store it in
             * device structure
             */
            parse_humidity_calib_data(calib_data, dev);
        }
    }

		dev->calib = *(struct bme280_calib_data *)calib_data;

    return rslt;
}