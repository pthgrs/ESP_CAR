#ifndef __HEADER__
#define __HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpiod_if2.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "MQTTClient.h"

// mode
#define MODE_MANUAL     0
#define MODE_AUTOREC    10
#define MODE_AUTO       11
#define MODE_FACE       20

//right
#define MA1     5 
#define MB1     6
#define EN1     23
// left
#define MA2     27
#define MB2     17
#define EN2     24

// hcsr04
#define TRIG_PINNO  26
#define ECHO_PINNO  19

// DHT22
#define SDAPIN  20
#define LED     21

#define LEN     50

/* setting */
int pi;
MQTTClient subClient;
MQTTClient pubClient;

/* racing data */
int moveMode;
char direction[LEN];
int speed;
time_t start_time, end_time;
FILE *fp;

#endif

/* direction */
int initMotor();  
void move_go();
void move_back();
void move_right();
void move_left();
void move_stop();
void controlSpeed(int sp);

/* sensors */
void initHcsr04(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
void trigger(void);
void *hcsr04_run(void *p);

void initDHT22(void);
void cb_func_dht22(int pi, unsigned user_gpio, unsigned level, uint32_t tick);
void read_dht_data(void);
void *dht22_run(void *p);
void *alertLED(void *p);

/* MQTTClient */
void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message    );
void connlost(void *context, char *cause);
void *sub(void *p);
void initPub();
void publish(char *topic, char *msg);

/* Controller */
void msgController(char *topicName, char *msg);
void speedController(char *msg);
void directionController(char *msg);

/* record */
void start_record();
void end_record();
void recording();
void run_recording();
float getTimeDiff();
