#include "project.h"

int initMotor()
{
    int default_range = 100;

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

void go_forward()
{
    gpio_write(pi, MA1, PI_HIGH);
    gpio_write(pi, MB1, PI_LOW);
    gpio_write(pi, EN1, PI_HIGH);

    gpio_write(pi, MA2, PI_HIGH);
    gpio_write(pi, MB2, PI_LOW);
    gpio_write(pi, EN2, PI_HIGH);
}

void go_backward()
{
    gpio_write(pi, MA1, PI_LOW);
    gpio_write(pi, MB1, PI_HIGH);
    gpio_write(pi, EN1, PI_HIGH);

    gpio_write(pi, MA2, PI_LOW);
    gpio_write(pi, MB2, PI_HIGH);
    gpio_write(pi, EN2, PI_HIGH);
}

void go_right()
{
    gpio_write(pi, MA1, PI_LOW);
    gpio_write(pi, MB1, PI_LOW);
    gpio_write(pi, EN1, PI_HIGH);

    gpio_write(pi, MA2, PI_HIGH);
    gpio_write(pi, MB2, PI_LOW);
    gpio_write(pi, EN2, PI_HIGH);
}

void go_left()
{
    gpio_write(pi, MA1, PI_HIGH);
    gpio_write(pi, MB1, PI_LOW);
    gpio_write(pi, EN1, PI_HIGH);

    gpio_write(pi, MA2, PI_LOW);
    gpio_write(pi, MB2, PI_LOW);
    gpio_write(pi, EN2, PI_HIGH);
}

void stop()
{
    gpio_write(pi, MA1, PI_LOW);
    gpio_write(pi, MB1, PI_LOW);
    gpio_write(pi, EN1, PI_LOW);

    gpio_write(pi, MA2, PI_LOW);
    gpio_write(pi, MB2, PI_LOW);
    gpio_write(pi, EN2, PI_LOW);
}

void controlSpeed(int sp)
{
    set_PWM_dutycycle(pi, EN1, sp);
    set_PWM_dutycycle(pi, EN2, sp);
}
