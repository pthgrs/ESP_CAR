#include "project.h"

int main()
{
    pthread_t tid1, tid2;

    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    moveMode = MODE_MANUAL;

    // motor
    initMotor();

    // hcsr04
    initHcsr04();
    pthread_create(&tid1, NULL, hcsr04_run, NULL);

    // DHT22
//    read_dht_data();
    pthread_create(&tid2, NULL, dht22_run, NULL); 

    // mqtt
    createMQTTClient();

    // pigpio_stop(pi);

}
