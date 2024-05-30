#include "fsl_spi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdbool.h>

#define EXAMPLE_SPI_MASTER          SPI0
#define EXAMPLE_CLK_SRC             kCLOCK_MainClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFreq(EXAMPLE_CLK_SRC)
#define EXAMPLE_SPI_MASTER_BAUDRATE 500000U
#define EXAMPLE_SPI_MASTER_SSEL     kSPI_Ssel0Assert

void MCP23S17_write(uint8_t address, uint8_t b0, uint8_t b1) {
	uint8_t txBuffer[3];
	uint8_t rxBuffer[3];

	spi_transfer_t xfer = { 0 };

	txBuffer[0] = address;
	txBuffer[1] = b0;
	txBuffer[2] = b1;

	xfer.txData = txBuffer;
	xfer.rxData = rxBuffer;
	xfer.dataSize = sizeof(txBuffer);
	xfer.configFlags = kSPI_EndOfTransfer | kSPI_EndOfFrame;

	SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);
}

void MCP23S17_spi_init(uint32_t baud_rate) {
	spi_master_config_t userConfig = { 0 };
	uint32_t srcFreq = 0U;

	SPI_MasterGetDefaultConfig(&userConfig);
	userConfig.baudRate_Bps = baud_rate;
	userConfig.clockPhase = kSPI_ClockPhaseFirstEdge;
	userConfig.delayConfig.frameDelay = 0;
	userConfig.clockPolarity = kSPI_ClockPolarityActiveHigh;
	userConfig.dataWidth = kSPI_Data8Bits;
	srcFreq = EXAMPLE_SPI_MASTER_CLK_FREQ;
	SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);

}
