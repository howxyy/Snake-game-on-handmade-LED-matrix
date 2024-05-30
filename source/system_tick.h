#ifndef SYSTEM_TICK_H_
#define SYSTEM_TICK_H_

#include "joystick.h"

void system_tick_init();
void system_tick_run();
void system_tick_delay_ms(uint32_t ms);

#endif /* SYSTEM_TICK_H_ */
