#include "project.h"

void initServo(int pi, int gpio){
	set_mode(pi, gpio, PI_OUTPUT);
}

// 30 <= degree =< 150
void moveServo(int degree){
	if(degree >= 30 && degree <= 150){
		degree -= 10;
		int pulse_width = (degree * 11.1111) + 500;
		set_servo_pulsewidth(pi, SERVO_GPIO, pulse_width);
	}
}
