#include "project.h"

int main()
{
    pthread_t tid1, tid2;
    pthread_t tid3;

    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    // motor
    initMotor();

    // mqtt
    pthread_create(&tid3, NULL, sub, NULL);
    initPub();

    // hcsr04
    initHcsr04();
    pthread_create(&tid1, NULL, hcsr04_run, NULL);

    // DHT22
    read_dht_data();
    pthread_create(&tid2, NULL, dht22_run, NULL); 
    
    while(1);
    // pigpio_stop(pi);
}
