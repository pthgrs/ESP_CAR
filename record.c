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
	
//			printf("%s\t%d\t%lf\n", dir, sp, diff);
			if(moveMode != MODE_AUTO){
				printf("in run record! is not MODE_AUTO\n");
//				move_stop();
				gpio_write(pi, MA1, PI_LOW);
				gpio_write(pi, MA2, PI_LOW);
				gpio_write(pi, MB1, PI_LOW);
				gpio_write(pi, MB2, PI_LOW);

				return NULL;
			}

			controlSpeed(sp);
			directionController(dir);
			usleep(diff - 200);
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
	diff = 1000000 *(double)(end.tv_sec - bgn.tv_sec)
		+(double)(end.tv_usec - bgn.tv_usec);

	gettimeofday(&bgn, NULL);
	return diff;
}
