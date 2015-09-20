#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/mman.h>
#include "head/kernel_list.h"
#include <pthread.h>
#include <signal.h>
#include <linux/input.h>
#include <semaphore.h>
#include "head/motion.h"

int flag=1;
int flag_t = 1;
int x ,y,pressure;
sem_t  first,second;

void *musicplay(void *arg)
{
	int i ,count;
	char mp3_file[10][40];
	char command[30];
	printf("enter musicplay\n");
	count = read_dir("/mp3",".mp3",&mp3_file);
	printf(" read mp3 file ret = %d \n",count);
	for(i=0;i<count;i++)
	{
		sprintf(command,"madplay %s\n",mp3_file[i]);
		system(command);
	}
	pthread_exit("play music is done");
	
}

void *bmpshow(void *arg)
{
	printf("enter bmpshow\n");
	
	printf("flag_bmp =%d\n",flag);
	int lcd_fd;
	lcd_fd= open("/dev/fb0", O_RDWR);
	if(lcd_fd<0)
	{
		printf(" open LCD Failed!\n");
		return  (void*)-1;
	}
	list_show(lcd_fd, flag);
	pthread_exit("bmp show is done");
	close(lcd_fd);
	
}

void *harvest(void *arg)
{
	printf("enter harvest\n");
	
	int env_fd = open("/dev/event0", O_RDONLY);
	if(env_fd<0)
	{
		printf(" open event0 Failed!\n");
		return  (void*)-1;
	}
	struct input_event *buf = malloc(sizeof(struct input_event));
	sem_wait(&second);	      //first harvest 
	while(1)
	{
		int len = sizeof(*buf);		
		bzero(buf, len);	 
		read(env_fd, buf, len);
		
		if(buf->type == EV_ABS)		
		{
			if(buf->code == ABS_X)
				{
					printf("(%d, ", buf->value);	
					x= buf->value;
				}
			else if(buf->code == ABS_Y)
				{
					printf("%d):", buf->value);
					y = buf->value;
				}
			else if(buf->code == ABS_PRESSURE)
				{
					printf("%d\n", buf->value);
					pressure = buf->value;		
				}			 
		}
		if(flag_t == 1)     //mark so that harvest one
			{
				sem_post(&first);
				//flag_t = 0;
			}
			
		 
	}
	close(env_fd);
	
	pthread_exit("harvest is done\n");

	
}

int get_motion()
{
	int x1,y1,x2,y2;
	pthread_t  thid_har;
	void *har_cor;

	 sem_init(&first,0,0); 
	 sem_init(&second,0,1);
	
	 int lcd_fd;
	printf(".........................\n");
	
	if(pthread_create(&thid_har,NULL,harvest,NULL) != 0)
	{
		perror("create thid_har failed\n");
	}
	while(1)
	{
		sem_wait(&first);
		x1 = x;
		y1 = y;
		flag_t = 0;
		if(pressure)
			{
				sleep(2);
				if(pressure)
					flag = AUTOPLAY;  //restart auto play bmp
				else 
					flag = STOP;  //stop play bmp
			}
		sem_post(&second);
		
		while(1)
		{
			if(pressure == 0)
			{			
				usleep(200000);
				if(pressure == 0)
				{
					x2 = x;
					y2 = y;	
					/*if(pthread_cancel(thid_har) != 0)       //this will cause wrong?why?
						printf("cacel() error");*/
					break;
				}
			}
			 
		}
		printf("x1=%d\n",x1);
		printf("x2=%d\n",x2);
		printf("y1=%d\n",y1);
		printf("y2=%d\n",y2);
		int delta_x = x1-x2 > 0 ? x1-x2 : x2-x1;
		int delta_y = y1-y2 > 0 ? y1-y2 : y2-y1;
		printf(".........................\n");
		printf("delta_x = %d\n",delta_x);
		printf("delta_y = %d\n",delta_y);
		
		if(x1 == 0 || x2 == 0)
			return -1;

		if(x1>x2 && delta_x > delta_y)
				flag = LEFT;		//left	  prev
		else if(x2>x1 && delta_x > delta_y)
				flag = RIGHT;   			//right  nest
		else if(y1>y2 && delta_y > delta_x)
				flag = DOWM;
		/*else if(y2>y1 && delta_y > delta_x)
			return DOWM;
		/*else
			return -1;*/
			sem_post(&second);
			flag_t = 1;
			
			
	}	
	pthread_join(thid_har,&har_cor);
	printf(" %s\n",(char*)har_cor); 
			
		
	 	
}
int main(int argc, char **argv)
{

	int  env_fd,lcd_fd;
	int ret,count,i;
	pid_t pid;
	pthread_t thid1,thid2,thid_har;;
	
	void	*bmp,*music ;
	int x1,x2,y1,y2,press;

	

	
	printf(" welcome to My Ehome !\n ");
	
	
	pthread_create(&thid1,NULL,musicplay,NULL);
	pthread_create(&thid2,NULL,bmpshow,NULL);

 	
	get_motion();
		
		
 	

	

    //  ÊÍ·Å×ÊÔ´	
    pthread_join(thid1, &music);
	printf(" %s\n",(char*)music); 
	pthread_join(thid2, &bmp);
	printf(" %s\n",(char*)bmp); 
	
	close(env_fd);
	 

	return 0;
}



