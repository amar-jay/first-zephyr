#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>


#define ADC_CHANNEL DT_ALIAS(useradcch)
#define ADC_DEVICE DT_ALIAS(useradc)




// Get Devicetree configurations
#if !DT_NODE_HAS_STATUS(ADC_CHANNEL, okay)
  #error "Unsupported board: adc channel devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(ADC_DEVICE, okay)
	#error "Unsupported board: adc device devicetree alias is not defined"
#endif

static const struct device *adc_dev = DEVICE_DT_GET(ADC_DEVICE);
static const struct adc_channel_cfg adc_channel = ADC_CHANNEL_CFG_DT(ADC_CHANNEL);



int adc_read_val(void)
{

		uint16_t adc_sample;
    // Get Vref (mV) from Devicetree property
    int32_t vref_mv = DT_PROP(ADC_CHANNEL, zephyr_vref_mv);
    uint16_t mv = 0;


    // Make sure that the ADC was initialized
    if (!device_is_ready(adc_dev)) {
        printk("ADC peripheral is not ready\r\n");
        return 0;
    }

    // Configure ADC channel
    int ret = adc_channel_setup(adc_dev, &adc_channel);
    if (ret < 0) {
        printk("Could not set up ADC\r\n");
        return 0;
    }

		const struct adc_sequence sequence = {
			.channels = BIT(adc_channel.channel_id),
			.buffer = &adc_sample,
			.buffer_size = sizeof(adc_sample),
			.resolution = DT_PROP(ADC_CHANNEL, zephyr_resolution),
		};

		// Do forever
		while (1) {
			// Read ADC value
			ret = adc_read(adc_dev, &sequence);
	    if (ret < 0) {
          printk("Could not read ADC: %d\r\n", ret);
          continue;
      }

			// Print the raw ADC value and the voltage in mV
			mv = adc_sample * vref_mv / (1 << sequence.resolution);

			printk("ADC raw: %d, voltage: %d mV\r\n", adc_sample, mv);
			k_sleep(K_MSEC(1000));
		}
		return 0;
}