#include "fsl_debug_console.h"
#include "MCP23S17.h"
#include "matrix.h"

uint8_t byte_reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void matrix_write_spi(uint8_t row, uint8_t blue, uint8_t green, uint8_t red) {
  MCP23S17_write(MCP23S17_CHIP0, MCP23S17_GPIOA, 0xFF);
  MCP23S17_write(MCP23S17_CHIP1, MCP23S17_GPIOB, byte_reverse(blue));
  MCP23S17_write(MCP23S17_CHIP0, MCP23S17_GPIOB, byte_reverse(red));
  MCP23S17_write(MCP23S17_CHIP1, MCP23S17_GPIOA, byte_reverse(green));
  MCP23S17_write(MCP23S17_CHIP0, MCP23S17_GPIOA, row);
}


void matrix_draw(color_e board[MATRIX_SIZE][MATRIX_SIZE]) {
  for (int row = 0; row < MATRIX_SIZE+1; row++) {
    uint8_t porta = ~(1 << row), portb = 0xFF, portc = 0xFF, portd = 0xFF;
    for (int col = 0; col < MATRIX_SIZE; col++) {
      if (HAS_COLOR(board[row][col], BLUE_BIT)) {
        portb &= ~(1 << col);
      }
      if (HAS_COLOR(board[row][col], GREEN_BIT)) {
        portc &= ~(1 << col);
      }
      if (HAS_COLOR(board[row][col], RED_BIT)) {
        portd &= ~(1 << col);
      }
    }
    matrix_write_spi(porta, portb, portc, portd);
  }
}
