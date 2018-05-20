#include "project.h"

int main()
{
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    //motor
    initMotor();

    // mqtt
    createMQTTClient();
}
