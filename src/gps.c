#ifdef GNSS
#include <zephyr/drivers/gnss.h>

#define GNSS_MODEM DEVICE_DT_GET(DT_ALIAS(gnss))

#ifndef GPS_DEFINE
#define GPS_DEFINE() \
	static void gnss_data_cb(const struct device *dev, const struct gnss_data *data) \
	{ \
		int64_t timepulse_ns; \
		int64_t timepulse; \
		if (data->info.fix_status != GNSS_FIX_STATUS_NO_FIX) { \
			if (gnss_get_latest_timepulse(dev, &timepulse) == 0) { \
				timepulse_ns = k_ticks_to_ns_near64(timepulse); \
				LOG_INF("Got a fix (type: %d) @ %lld ns\n", data->info.fix_status, \
				       timepulse_ns); \
			} else { \
				LOG_INF("Got a fix (type: %d)\n", data->info.fix_status); \
			} \
			LOG_INF("date: %02d:%02d:%02d:%03d %02d-%02d-%04d", \
					data->utc.hour, data->utc.minute, data->utc.millisecond/1000, data->utc.millisecond%1000, \
					data->utc.month_day, data->utc.month, data->utc.century_year + 2000); \
 \
			LOG_INF("%s location (%lld, %lld) with %d tracked satellites", \
					dev->name, data->nav_data.latitude, data->nav_data.longitude, data->info.satellites_cnt); \
		} else { \
			LOG_INF("No fix (Satellites: %u) Qual:%d Status:%d HDOP:%d Sep:%d", \
				data->info.satellites_cnt, data->info.fix_quality, data->info.fix_status, \
				data->info.hdop, data->info.geoid_separation); \
		} \
	} \
	GNSS_DATA_CALLBACK_DEFINE(DEVICE_DT_GET(DT_ALIAS(gnss)), gnss_data_cb)
#endif // GPS_DEFINE


int get_gps_info(void) {	
	gnss_systems_t supported, enabled;
	uint32_t fix_interval;
	int rc;

	rc = gnss_get_fix_rate(GNSS_MODEM, &fix_interval);
	if (rc < 0) {
		LOG_ERR("Failed to query fix rate (%d)\n", rc);
		return rc;
	}

	rc = gnss_get_supported_systems(GNSS_MODEM, &supported);
	if (rc < 0) {
		printf("failed to query supported systems (%d)\n", rc);
		return rc;
	}

	rc = gnss_get_enabled_systems(GNSS_MODEM, &enabled);
	if (rc < 0) {
		printf("failed to query enabled systems (%d)\n", rc);
		return rc;
	}

	LOG_INF("- GNSS Fix rate: %d ms\n", fix_interval);
	LOG_INF("- GNSS Supported systems: %s%s%s%s%s%s%s%s\r\n", 
		supported & GNSS_SYSTEM_GPS ? "GPS " : "",
		supported & GNSS_SYSTEM_GLONASS ? "GLONASS " : "",
		supported & GNSS_SYSTEM_GALILEO ? "GALILEO " : "",
		supported & GNSS_SYSTEM_BEIDOU ? "BEIDOU " : "",
		supported & GNSS_SYSTEM_QZSS ? "QZSS " : "",
		supported & GNSS_SYSTEM_IRNSS ? "IRNS " : "",
		supported & GNSS_SYSTEM_SBAS ? "SBAS " : "",
		supported & GNSS_SYSTEM_IMES ? "IMES " : ""
	);
	LOG_INF("- GNSS Enabled systems: %s%s%s%s%s%s%s%s\n", 
		enabled & GNSS_SYSTEM_GPS ? "GPS " : "",
		enabled & GNSS_SYSTEM_GLONASS ? "GLONASS " : "",
		enabled & GNSS_SYSTEM_GALILEO ? "GALILEO " : "",
		enabled & GNSS_SYSTEM_BEIDOU ? "BEIDOU " : "",
		enabled & GNSS_SYSTEM_QZSS ? "QZSS " : "",
		enabled & GNSS_SYSTEM_IRNSS ? "IRNS " : "",
		enabled & GNSS_SYSTEM_SBAS ? "SBAS " : "",
		enabled & GNSS_SYSTEM_IMES ? "IMES " : ""
	);
	return 0;
}
#endif // GNSS