
#include "fsl_spi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "fsl_common_arm.h"

#include "buzzer.h"

#define BUZZER_PORT 	0
#define BUZZER_PIN		17

static volatile uint32_t g_counter = 0;

void buzzer_init() {
	GPIO_PortInit(GPIO, BUZZER_PORT);
	gpio_pin_config_t buzzer_config = { kGPIO_DigitalOutput, off, };
	GPIO_PinInit(GPIO, BUZZER_PORT, BUZZER_PIN, &buzzer_config);
}

void buzzer_set(buzzer_state_e state) {
	GPIO_PinWrite(GPIO, BUZZER_PORT, BUZZER_PIN, state);
}
