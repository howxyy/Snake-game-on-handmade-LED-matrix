#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdbool.h>

typedef enum {
	joystick_none,
	joystick_left,
	joystick_right,
	joystick_up,
	joystick_down,
} joystick_e;

typedef struct {
	int x;
	int y;
} adc_xy_t;

void adc_read(adc_xy_t *adc_xy);
void joystick_init();
joystick_e joystick_scan();
void joystick_prints(joystick_e input);
bool joystick_button();

#endif
