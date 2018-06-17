#include "project.h"

int initMotor()
{
    int default_range = 100;
	speed = 0;

    // setmode
    set_mode(pi, MA1, PI_OUTPUT); 
    set_mode(pi, MB1, PI_OUTPUT); 
    set_mode(pi, EN1, PI_OUTPUT); 

    set_mode(pi, MA2, PI_OUTPUT); 
    set_mode(pi, MB2, PI_OUTPUT); 
    set_mode(pi, EN2, PI_OUTPUT); 

    // PWM
    set_PWM_range(pi, EN1, default_range);
    set_PWM_range(pi, EN2, default_range);

    set_PWM_dutycycle(pi, EN1, 0);
    set_PWM_dutycycle(pi, EN2, 0);
}

void move_go()
{
	printf("move_go!!\n");
    gpio_write(pi, MA1, PI_HIGH);
    gpio_write(pi, MB1, PI_LOW);

    gpio_write(pi, MA2, PI_HIGH);
    gpio_write(pi, MB2, PI_LOW);
}

void move_back()
{
	printf("move_back!!\n");
    gpio_write(pi, MA1, PI_LOW);
    gpio_write(pi, MB1, PI_HIGH);

    gpio_write(pi, MA2, PI_LOW);
    gpio_write(pi, MB2, PI_HIGH);
}

void move_right()
{
	printf("move_right!!\n");
    gpio_write(pi, MA1, PI_LOW);
    gpio_write(pi, MB1, PI_LOW);

    gpio_write(pi, MA2, PI_HIGH);
    gpio_write(pi, MB2, PI_LOW);
}

void move_left()
{
	printf("move_left\n");
    gpio_write(pi, MA1, PI_HIGH);
    gpio_write(pi, MB1, PI_LOW);

    gpio_write(pi, MA2, PI_LOW);
    gpio_write(pi, MB2, PI_LOW);
}

void move_stop()
{
//	printf("in move_stop\n");
    gpio_write(pi, MA1, PI_LOW);
    gpio_write(pi, MB1, PI_LOW);

    gpio_write(pi, MA2, PI_LOW);
    gpio_write(pi, MB2, PI_LOW);
}

void controlSpeed(int sp)
{
    set_PWM_dutycycle(pi, EN1, sp);
    set_PWM_dutycycle(pi, EN2, sp);
}
