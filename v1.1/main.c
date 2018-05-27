#include "project.h"

int main()
{
    pthread_t tid1, tid2;
    
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    // motor
    initMotor();

    // DHT22
    pthread_create(&tid1, NULL, dht22_run, NULL); 

    // hcsr04
    initHcsr04();
    pthread_create(&tid2, NULL, hcsr04_run, NULL);

    // mqtt
    createMQTTClient();

    // pigpio_stop(pi);
}
