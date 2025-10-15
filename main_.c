#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

/**
Simple esp32-basic sample showing DTS overlay, GPIO, ADC and PWM usage 
*/
LOG_MODULE_REGISTER(esp32_basic, LOG_LEVEL_INF);

/* Use standard aliases used by Zephyr samples: led0 and pwm_led0. The overlay
 * in this sample declares these aliases. If you adapt the overlay, update
 * these macros accordingly.
 */
#define LED0_NODE DT_ALIAS(led0)
#define PWM_LED_NODE DT_ALIAS(pwm_led0)
#define ADC_NODE DT_ALIAS(adc0)

/* GPIO DT spec for led0 */
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#warning "LED0 alias not defined in devicetree"
static const struct gpio_dt_spec led = {0};
#endif

/* PWM DT spec for pwm_led0 */
#if DT_NODE_HAS_STATUS(PWM_LED_NODE, okay)
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED_NODE);
#else
#warning "PWM LED alias not defined in devicetree"
static const struct pwm_dt_spec pwm_led = {0};
#endif

/* ADC: basic single-channel example. For real board mappings, consider
 * defining ADC channel children in the devicetree and using the
 * ADC_CHANNEL_CFG_DT helper. For this simple demo we configure one
 * channel in code (channel 0) — edit as needed for your SoC.
 */

#if DT_NODE_HAS_STATUS(ADC_NODE, okay)
static const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);
#else
static const struct device *adc_dev = NULL;
#warning "ADC alias not defined in devicetree"
#endif

static struct adc_channel_cfg adc_chan_cfg = {
    .gain = ADC_GAIN_1,
    .reference = ADC_REF_INTERNAL,
    .acquisition_time = ADC_ACQ_TIME_DEFAULT,
    .channel_id = 0,
};

static int16_t sample_buffer;
static struct adc_sequence sequence = {
    .channels = BIT(0),
    .buffer = &sample_buffer,
    .buffer_size = sizeof(sample_buffer),
    .resolution = 12,
};

int main(void)
{
    int ret;

    LOG_INF("esp32-basic starting");

    /* GPIO (LED) */
    if (gpio_is_ready_dt(&led)) {
        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
        if (ret < 0) {
            LOG_ERR("Failed to configure LED: %d", ret);
        }
    } else {
        LOG_WRN("LED gpio not ready or not defined in DTS");
    }

    /* PWM */
    if (pwm_is_ready_dt(&pwm_led)) {
        /* Try set a moderate period and 50% duty as initial example */
        uint32_t period = PWM_USEC(2000U); /* 2 ms */
        ret = pwm_set_dt(&pwm_led, period, period / 2U);
        if (ret) {
            LOG_ERR("Failed to set initial PWM: %d", ret);
        }
    } else {
        LOG_WRN("PWM device not ready or not defined in DTS");
    }

    /* ADC setup */
    if (adc_dev) {
        if (!device_is_ready(adc_dev)) {
            LOG_ERR("ADC device not ready");
            adc_dev = NULL;
        } else {
            ret = adc_channel_setup(adc_dev, &adc_chan_cfg);
            if (ret < 0) {
                LOG_ERR("ADC channel setup failed: %d", ret);
                adc_dev = NULL;
            }
        }
    }

    while (1) {
        /* Toggle LED via DT helpers */
        if (gpio_is_ready_dt(&led)) {
            ret = gpio_pin_toggle_dt(&led);
            if (ret < 0) {
                LOG_ERR("Failed to toggle LED: %d", ret);
            } else {
                LOG_INF("LED toggled");
            }
        }

        /* Read ADC */
        if (adc_dev) {
            ret = adc_read(adc_dev, &sequence);
            if (ret == 0) {
                int32_t milliv;
                if (adc_raw_to_millivolts(ADC_REF_INTERNAL, adc_chan_cfg.gain, sequence.resolution, &milliv) == 0) {
                    LOG_INF("ADC raw: %d (mV approx: %d)", sample_buffer, (int)milliv);
                } else {
                    LOG_INF("ADC raw: %d", sample_buffer);
                }
            } else {
                LOG_ERR("ADC read failed: %d", ret);
            }
        }

        /* Ramp PWM duty (if available) */
        if (pwm_is_ready_dt(&pwm_led)) {
            static uint8_t duty = 0;
            uint32_t period = PWM_USEC(2000U);
            uint32_t pulse = (period * duty) / 100U;
            ret = pwm_set_dt(&pwm_led, period, pulse);
            if (ret) {
                LOG_ERR("PWM set failed: %d", ret);
            } else {
                LOG_INF("PWM duty %u%%", duty);
            }
            duty += 25;
            if (duty > 100) {
                duty = 0;
            }
        }

        k_sleep(K_SECONDS(2));
    }
		return 0;
}
