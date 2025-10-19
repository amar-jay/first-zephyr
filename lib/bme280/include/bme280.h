
/**
* Documentation: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
* Author: amar-jay<abdelmanan.abdelrahman03@gmail.com>
*/

#ifndef BME280_H_
#define BME280_H_
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

typedef enum {
	BME280_H_OVERSAMPLING_SKIP = 0x00,
	BME280_H_OVERSAMPLING_X1   = 0x01,
	BME280_H_OVERSAMPLING_X2   = 0x02,
	BME280_H_OVERSAMPLING_X4   = 0x03,
	BME280_H_OVERSAMPLING_X8   = 0x04,
	BME280_H_OVERSAMPLING_X16  = 0x05,
} bme280_oversampling_t;

/*!
 * @brief Calibration data
 */
struct bme280_calib_data
{
    /*! Calibration coefficient for the temperature sensor */
    uint16_t dig_t1;

    /*! Calibration coefficient for the temperature sensor */
    int16_t dig_t2;

    /*! Calibration coefficient for the temperature sensor */
    int16_t dig_t3;

    /*! Calibration coefficient for the pressure sensor */
    uint16_t dig_p1;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p2;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p3;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p4;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p5;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p6;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p7;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p8;

    /*! Calibration coefficient for the pressure sensor */
    int16_t dig_p9;

    /*! Calibration coefficient for the humidity sensor */
    uint8_t dig_h1;

    /*! Calibration coefficient for the humidity sensor */
    int16_t dig_h2;

    /*! Calibration coefficient for the humidity sensor */
    uint8_t dig_h3;

    /*! Calibration coefficient for the humidity sensor */
    int16_t dig_h4;

    /*! Calibration coefficient for the humidity sensor */
    int16_t dig_h5;

    /*! Calibration coefficient for the humidity sensor */
    int8_t dig_h6;

    /*! Variable to store the intermediate temperature coefficient */
    int32_t t_fine;
};

struct bme280_dev {
	const struct device *i2c_dev;
	uint8_t i2c_addr;
	bme280_oversampling_t osrs_t; // Temperature oversampling
	bme280_oversampling_t osrs_p; // Pressure oversampling
	struct bme280_calib_data calib;
};

typedef enum {
	BME280_SLEEP_MODE  = 0x00,
	BME280_FORCED_MODE = 0x01,
	BME280_NORMAL_MODE = 0x03,
} bme280_mode_t;

typedef enum {
	BME280_STANDBY_0_5_MS  = 0b000,
	BME280_STANDBY_62_5_MS = 0b001,
	BME280_STANDBY_125_MS  = 0b010,
	BME280_STANDBY_250_MS  = 0b011,
	BME280_STANDBY_500_MS  = 0b100,
	BME280_STANDBY_1000_MS = 0b101,
	BME280_STANDBY_10_MS   = 0b110,
	BME280_STANDBY_20_MS   = 0b111,
} bme280_standby_time_t;

typedef enum {
	BME280_FILTER_OFF       = 0b000,
	BME280_FILTER_COEFF_2   = 0b001,
	BME280_FILTER_COEFF_4   = 0b010,
	BME280_FILTER_COEFF_8   = 0b011,
	BME280_FILTER_COEFF_16  = 0b100,
} bme280_filter_coefficient_t;

/*!
 * @brief bme280 sensor structure which comprises of uncompensated temperature,
 * pressure and humidity data
 */
struct bme280_uncomp_data
{
    uint32_t pressure;
    uint32_t temperature;
    uint32_t humidity;
};

// Function prototypes
/**
 * @brief Initialize the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param i2c_dev Pointer to the I2C device structure.
 * @param i2c_addr I2C address of the BME280 sensor (use 0 to auto-detect).
 * @return 0 on success, negative error code on failure.
 */
int bme280_init(struct bme280_dev *dev, const struct device *i2c_dev, uint8_t i2c_addr);
/**
 * @brief Deinitialize the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 */
void bme280_deinit(struct bme280_dev *dev);
/**
 * @brief Set the mode for the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param mode The desired mode setting.
 * @return 0 on success, negative error code on failure.
 */
int bme280_set_mode(struct bme280_dev *dev, uint8_t mode);
/**
 * @brief Set the temperature oversampling for the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param osrs_t The desired temperature oversampling setting.
 * @return 0 on success, negative error code on failure.
 */
int bme280_set_temp_sampling(struct bme280_dev *dev, bme280_oversampling_t osrs_t);
/**
 * @brief Set the pressure oversampling for the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param osrs_p The desired pressure oversampling setting.
 * @return 0 on success, negative error code on failure.
 */
int bme280_set_pressure_sampling(struct bme280_dev *dev, bme280_oversampling_t osrs_p);

/**
 * @brief Set the humidity oversampling for the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param osrs_h The desired humidity oversampling setting.
 * @return 0 on success, negative error code on failure.
 */
int bme280_set_humidity_sampling(struct bme280_dev *dev, bme280_oversampling_t osrs_h);

/**
 * @brief Configure the BME280 sensor with standby time and filter coefficient.
 * @param dev Pointer to the BME280 device structure.
 * @param standby_time The desired standby time setting.
 * @param filter_coefficient The desired filter coefficient setting.
 * @return 0 on success, negative error code on failure.
 */
int bme280_configure(struct bme280_dev *dev, bme280_standby_time_t standby_time, bme280_filter_coefficient_t filter_coefficient);

/**
 * @brief Read the temperature from the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param temp Pointer to the variable to store the temperature value.
 * @return 0 on success, negative error code on failure.
 */
#ifdef BME280_DOUBLE_ENABLE
double bme280_read_temp(struct bme280_dev *dev, int32_t *temp);
#else
uint32_t bme280_read_temp(struct bme280_dev *dev, int32_t *temp);
#endif

/**
 * @brief Read the humidity from the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param humidity Pointer to the variable to store the humidity value.
 * @return 0 on success, negative error code on failure.
 */
#ifdef BME280_DOUBLE_ENABLE
double bme280_read_humidity(struct bme280_dev *dev, int32_t *humidity);
#else
uint32_t bme280_read_humidity(struct bme280_dev *dev, int32_t *humidity);
#endif

/**
 * @brief Read the pressure from the BME280 sensor.
 * @param dev Pointer to the BME280 device structure.
 * @param pressure Pointer to the variable to store the pressure value.
 * @return 0 on success, negative error code on failure.
 */
#ifdef BME280_DOUBLE_ENABLE
double bme280_read_pressure(struct bme280_dev *dev, int32_t *pressure);
#else
uint32_t bme280_read_pressure(struct bme280_dev *dev, int32_t *pressure);
#endif

/**
 * @brief Get the calibration data from the BME280 sensor into the device structure.
 * @param dev Pointer to the BME280 device structure.
 * @return 0 on success, negative error code on failure.
 */
int bme280_get_calib_data(struct bme280_dev *dev);

#endif /* BME280_H_ */