#ifndef MATRIX_H_
#define MATRIX_H_

#define MATRIX_SIZE 							8
#define RED_BIT                   (1<<2)
#define GREEN_BIT                 (1<<1)
#define BLUE_BIT                  (1<<0)
#define COLOR(c_bit)              ((c_bit)&7)
#define HAS_COLOR(color, c_bit)   ((~((int)(color)))&(c_bit)?1:0)

typedef enum {
  NONE = 0,                 // 000
  RED = COLOR(RED_BIT),     // 100
  GREEN = COLOR(GREEN_BIT), // 010
  BLUE = COLOR(BLUE_BIT),   // 001
  PURPLE = COLOR(BLUE_BIT+RED_BIT), // 101
  TEAL = COLOR(BLUE_BIT+GREEN_BIT), // 011
  LIGHTGREEN = COLOR(RED_BIT+GREEN_BIT), // 110
  WHITE = COLOR(RED_BIT + GREEN_BIT + BLUE_BIT) // 111
} color_e;

void matrix_write_spi(uint8_t row, uint8_t blue, uint8_t green, uint8_t red);
void matrix_draw(color_e board[MATRIX_SIZE][MATRIX_SIZE]);

#endif
