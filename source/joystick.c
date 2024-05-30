#include "fsl_spi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "joystick.h"
#include <stdbool.h>



#define ADC_MAX

#define HYS_TOP		(2096 + 1024)
#define HYS_BOT		(2096 - 1024)

#define DEMO_ADC_BASE                  	ADC0
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER1 1U
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER2 2U
#define DEMO_ADC_CLOCK_SOURCE          	kCLOCK_Irc
#define DEMO_ADC_CLOCK_DIVIDER         	1U
#define JOYSTICK_PORT 0
#define JOYSTICK_PIN 18

static adc_xy_t g_adc_xy_n1 = { 0 };
static int g_init_done = 0;

static void ADC_Configuration(void) {
	adc_config_t adcConfigStruct;
	adc_conv_seq_config_t adcConvSeqConfigStruct;

	adcConfigStruct.clockDividerNumber = DEMO_ADC_CLOCK_DIVIDER;
	adcConfigStruct.enableLowPowerMode = false;
	adcConfigStruct.voltageRange = kADC_HighVoltageRange;

	ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

	adcConvSeqConfigStruct.channelMask = (1U << DEMO_ADC_SAMPLE_CHANNEL_NUMBER1
			| 1U << DEMO_ADC_SAMPLE_CHANNEL_NUMBER2);
	adcConvSeqConfigStruct.triggerMask = 0U;
	adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
	adcConvSeqConfigStruct.enableSingleStep = false;
	adcConvSeqConfigStruct.enableSyncBypass = false;
	adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
	ADC_SetConvSeqAConfig(DEMO_ADC_BASE, &adcConvSeqConfigStruct);
	ADC_EnableConvSeqA(DEMO_ADC_BASE, true);
	ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);
}

void adc_read(adc_xy_t *adc_xy) {
	if(!g_init_done){
		printf("joystick read not allowed.\n");
		return;
	}
	adc_result_info_t adcResultInfoStruct;
	ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);
	ADC_GetChannelConversionResult(DEMO_ADC_BASE, DEMO_ADC_SAMPLE_CHANNEL_NUMBER2, &adcResultInfoStruct);
	adc_xy->y = adcResultInfoStruct.result;
	ADC_GetChannelConversionResult(DEMO_ADC_BASE, DEMO_ADC_SAMPLE_CHANNEL_NUMBER1, &adcResultInfoStruct);
	adc_xy->x = adcResultInfoStruct.result;
}

void joystick_init() {
	POWER_DisablePD(kPDRUNCFG_PD_ADC0);
	uint32_t frequency = 0U;

	frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE);
	if (true == ADC_DoSelfCalibration(DEMO_ADC_BASE, frequency)) {
		printf("ADC Calibration Done.\n");
	} else {
		printf("ADC Calibration Failed.\n");
	}
	GPIO_PortInit(GPIO, JOYSTICK_PORT);
	gpio_pin_config_t joystick_config = {kGPIO_DigitalInput};
	GPIO_PinInit(GPIO, JOYSTICK_PORT, JOYSTICK_PIN, &joystick_config);

	ADC_Configuration();
	printf("Configuration Done.\n");

	adc_read(&g_adc_xy_n1);
	g_init_done = 1;
}

bool joystick_button(){
	return GPIO_PinRead(GPIO, JOYSTICK_PORT, JOYSTICK_PIN);
}

joystick_e joystick_scan() {
	adc_xy_t adc_xy;
	joystick_e joystick = joystick_none;
	adc_read(&adc_xy);
	if (adc_xy.y > HYS_TOP) {
		joystick = joystick_up;
	} else if (adc_xy.y < HYS_BOT) {
		joystick = joystick_down;
	}

	if (adc_xy.x > HYS_TOP) {
		joystick = joystick_left;
	} else if (adc_xy.x < HYS_BOT) {
		joystick = joystick_right;
	}
	return joystick;
}

void joystick_prints(joystick_e input) {
	adc_xy_t adc_xy;
	adc_read(&adc_xy);

	printf("\nVertical:");
	printf("%d ", adc_xy.y);
	if (input == joystick_up)
		printf("Top");
	else if (input == joystick_down)
		printf("Bottom");
	else{
		printf("None");
	}

	printf(" - Horizontal:");
	printf("%d ", adc_xy.x);
	if (input == joystick_left)
		printf("Left");
	else if (input == joystick_right)
		printf("Right");
	else
		printf("None");
}

