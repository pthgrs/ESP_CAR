#include "project.h"

uint32_t h_start_tick_, dist_tick_;

void initHcsr04()
{
    float distance;

    set_mode(pi, TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, ECHO_PINNO, PI_INPUT);
    callback(pi, ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, TRIG_PINNO, PI_OFF);
    time_sleep(1);     // delay 1 second

    printf("Raspberry Pi HC-SR04 UltraSonic sensor\n" );
}

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    if(level == PI_HIGH)
        h_start_tick_ = tick;
    else if(level == PI_LOW)
        dist_tick_ = tick - h_start_tick_;
}

void *hcsr04_run(void *p)
{
    float distance;

    while(1){

        h_start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.05);

        if(dist_tick_ && h_start_tick_){

            distance = dist_tick_ / 1000000. * 340 / 2 * 100;

            if(distance > 400);
                //printf("range error\n");

            if(distance < 8 && strcmp(direction, "back")){
                //printf("distance : %6.1fcm\ttoo close\n", distance);
                move_stop();
            }
            else 
                ;
        }
        else;
           // printf("sense error\n");

        time_sleep(0.1);
    }
}
