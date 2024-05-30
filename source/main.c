#include "fsl_spi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "matrix.h"
#include "MCP23S17.h"
#include "joystick.h"
#include "buzzer.h"
#include "system_tick.h"
#include "snake.h"
#include "doublelinkedlist.h"
#include <time.h>

volatile uint32_t g_counter = 0;

void SysTick_Handler(void) {
	if ((++g_counter % 350) == 0) {
		system_tick_run();
	}
}
int main(void) {

	CLOCK_EnableClock(kCLOCK_Uart0);
	CLOCK_SetClkDivider(kCLOCK_DivUsartClk, 1U);

	CLOCK_EnableClock(kCLOCK_Spi0);

	CLOCK_EnableClock(kCLOCK_Adc);

	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	system_tick_init();
	joystick_init();
	buzzer_init();

	MCP23S17_spi_init(500000);

	MCP23S17_write(MCP23S17_CHIP_DEFAULT, MCP23S17_IOCON1, HAEN);
	MCP23S17_write(MCP23S17_CHIP0, MCP23S17_IODIRA, 0x00);
	MCP23S17_write(MCP23S17_CHIP0, MCP23S17_IODIRB, 0x00);
	MCP23S17_write(MCP23S17_CHIP1, MCP23S17_IODIRA, 0x00);
	MCP23S17_write(MCP23S17_CHIP1, MCP23S17_IODIRB, 0x00);

	start_game();
	while(1){
		matrix_draw(snake_matrix);
		joystick_input(joystick_scan());
	}
}




