#include "fsl_spi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "snake.h"
#include "system_tick.h"

static volatile uint32_t g_systickCounter = 0;
static volatile uint32_t g_init_done = 0;

void system_tick_init() {
	SysTick_Config(SystemCoreClock / 1200U);
	g_init_done = 1;
}

void system_tick_run() {
	if (!g_init_done) {
		printf("System tick run is not allowed.\n");
		return;
	}
	update_matrix();
	movement();
	check_move();
}

void system_tick_delay_ms(uint32_t ms) {
	if (!g_init_done) {
		printf("System tick delay is not allowed.\n");
		return;
	}
	g_systickCounter = 1000 * ms;
	while (g_systickCounter != 0U) {
		g_systickCounter--;
	}
}

