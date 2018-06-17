#include "project.h"

int pi;
MQTTClient subClient;
MQTTClient pubClient;

int moveMode;
int autoStop;
char direction[LEN];
int speed;
time_t start_time, end_time;
struct timeval bgn, end;
FILE *fp;

void* colorMove(void* arg){
	while(1){
		int go=0,back=0,left=0,right=0;

		if(moveMode == MODE_FACE){
			go = gpio_read(pi,12);
			back = gpio_read(pi,13);
			left = gpio_read(pi,16);
			right = gpio_read(pi,18);

			printf("%d %d %d %d\n", go, back, left, right);
			
			if(go == 0 && back == 0 && left == 0 && right == 0)
				move_stop();
			else{
				if(go) move_go();
				else if(back) move_back();
				if(left) move_left();
				else if(right) move_right();
			}

		}else continue;
	}
}


int main()
{
    pthread_t tid1, tid2;
    pthread_t tid3,tid4;


	gpio_write(pi, 5, PI_LOW);
	gpio_write(pi, 6, PI_LOW);
	gpio_write(pi, 23, PI_LOW);
	gpio_write(pi, 27, PI_LOW);
	gpio_write(pi, 17, PI_LOW);
	gpio_write(pi, 24, PI_LOW);

	set_mode(pi, LED, PI_OUTPUT);
	gpio_write(pi, LED, 0);

    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    // motor
    initMotor();
	autoStop = 0;
	//servo init(90 degree)
	moveServo(90);

    // mqtt
    pthread_create(&tid3, NULL, sub, NULL);
    initPub();

    // hcsr04
    initHcsr04();
    pthread_create(&tid1, NULL, hcsr04_run, NULL);

    // DHT22
    read_dht_data();
    pthread_create(&tid2, NULL, dht22_run, NULL); 

	pthread_create(&tid4, NULL, colorMove,NULL);
    
    while(1);
    // pigpio_stop(pi);
}
