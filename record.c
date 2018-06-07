#include "project.h"

void start_record()
{

    moveMode = MODE_AUTOREC;
    start_time = clock();

    if((fp = fopen("./log", "w")) == NULL)
        printf("openerror\n");
}

void recording()
{
    char str[100];
    int speedTmp = 20 + speed * 7;

    if(strlen(direction) == 0)
        return;

    sprintf(str, "%s\t%d\t%f\n", direction, speedTmp, getTimeDiff());
    fwrite(str, sizeof(char), strlen(str), fp);
}

void run_recording()
{
    char dir[50];
    int sp;
    float diff;

    if((fp = fopen("./log", "r")) == NULL)
        printf("openerr\n");

    while(fscanf(fp, "%s\t%d\t%f\n", dir, &sp, &diff) > 0){
        
        controlSpeed(sp);
        directionController(dir);
        usleep(diff);
    }
}

void end_record()
{
    fclose(fp);
}

float getTimeDiff()
{
    float diff;

    end_time = clock();
    diff = (float)(end_time - start_time);
    start_time = end_time;

    return diff;
}
