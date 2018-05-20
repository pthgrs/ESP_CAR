#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#include "MQTTClient.h"

//right
#define MA1     5 
#define MB1     6
#define EN1     23
// left
#define MA2     27
#define MB2     17
#define EN2     24


int pi;

/* direction */
int initMotor();  
void go_forward();
void go_backward();
void go_right();
void go_left();
void stop();
void controlSpeed(int sp);


/* MQTTClient */
void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message    );
void connlost(void *context, char *cause);
void createMQTTClient();

