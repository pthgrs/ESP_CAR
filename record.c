#include "project.h"

void start_record()
{

    moveMode = MODE_AUTOREC;
	gettimeofday(&bgn, NULL);
    //start_time = clock();

    if((fp = fopen("./log", "w")) == NULL)
        printf("openerror\n");
}

void recording()
{
    char str[100];

    if(strlen(direction) == 0)
        return;

    sprintf(str, "%s\t%d\t%f\n", direction, speed, getTimeDiff());
    fwrite(str, sizeof(char), strlen(str), fp);
}

void *run_recording(void *p)
{
    char dir[50];
    int sp;
    double diff;

    if((fp = fopen("./log", "r")) == NULL)
        printf("openerr\n");

	while(1){
		fseek(fp, 0, SEEK_SET);

	    while(fscanf(fp, "%s\t%d\t%lf\n", dir, &sp, &diff) > 0){
	
			if(moveMode != MODE_AUTO){
				move_stop();
				return NULL;
			}

			printf("%s\t%d\t%f\n", dir, sp, diff);
	//		controlSpeed(sp);
	//		directionController(dir);
			directionController(dir);
	//		controlSpeed(sp);
			set_PWM_dutycycle(pi, EN1, sp);
			set_PWM_dutycycle(pi, EN2, sp);
//			usleep(diff);
			sleep(diff);
	   }
	}
}

void end_record()
{
    fclose(fp);
}
/*
float getTimeDiff()
{
    float diff;

    end_time = clock();
    diff = (float)(end_time - start_time);
    start_time = end_time;

    return diff;
}
*/
double getTimeDiff()
{
	double diff;

	gettimeofday(&end, NULL);
	diff = (double)end.tv_sec + (double)end.tv_usec / 1000000.0 - (double)bgn.tv_sec - (double)bgn.tv_usec / 1000000.0;

	gettimeofday(&bgn, NULL);
	return diff;
}
