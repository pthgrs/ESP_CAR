#include "project.h"

void msgController(char *topicName, char *msg)
{
	pthread_t tid;

    if(strcmp(topicName, "direction") == 0)
        directionController(msg);

    else if(strcmp(topicName, "speed") == 0)
       speedController(msg);

	else if(strcmp(topicName, "degree") == 0){
		int degree = atoi(msg);
		moveServo(degree);
	}

    if(moveMode ==  MODE_AUTOREC)
        recording();

    // about MODE_AUTOREC
    if(strcmp(topicName, "moveMode") == 0){
        
        int mode = atoi(msg);

        
        if(mode == MODE_AUTOREC)
            start_record();
        
        else if(moveMode == MODE_AUTOREC && mode != MODE_AUTOREC)
            end_record();

		else if(mode == MODE_AUTO){
			autoStop = 0;
			printf("in msgController MODE_AUTO!!!!!!!!!\n");
            pthread_create(&tid, NULL, run_recording, NULL);
		}

        moveMode = mode;
    }

}

void directionController(char *msg)
{
	if(moveMode == MODE_AUTO && strcmp(msg, "stop") ==0)
		return;
    strcpy(direction, msg);

    if(strcmp(msg, "go") == 0)
        move_go();
    else if(strcmp(msg, "back") == 0)
        move_back();
    else if(strcmp(msg, "right") == 0)
        move_right();
    else if(strcmp(msg, "left") == 0)
        move_left();
    else if(strcmp(msg, "stop") == 0)
        move_stop();
}

void speedController(char *msg)
{
    // seekBar( 0, 1, 2 ...10)
    speed = 50 + atoi(msg) * 10;
	controlSpeed(speed);
}

