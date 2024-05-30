#ifndef SNAKE_H_
#define SNAKE_H_
#include "joystick.h"
#include "matrix.h"

extern color_e snake_matrix[MATRIX_SIZE][MATRIX_SIZE];
extern volatile uint32_t g_counter;

void movement();
void spawn_food();
void update_matrix();
void game_over();
void start_game();
void check_move();
void joystick_input (joystick_e input);

#endif
