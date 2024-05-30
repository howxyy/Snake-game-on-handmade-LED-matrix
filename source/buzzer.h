
#ifndef BUZZER_H_
#define BUZZER_H_

typedef enum {
	off = 0,
	on = 1,
} buzzer_state_e;

void buzzer_init();
void buzzer_set(buzzer_state_e state);

#endif /* BUZZER_H_ */
