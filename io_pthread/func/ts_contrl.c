#include <linux/input.h>
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
#include<sys/mman.h>
#include <pthread.h>
#include <signal.h>






int fd = open("/dev/event0", O_RDONLY);
	
	struct input_event *buf = malloc(sizeof(struct input_event));
	while(1)
	{
		int len = sizeof(*buf);
		bzero(buf, len);

		read(fd, buf, len);

		if(buf->type == EV_ABS)		
		{
			if(buf->code == ABS_X)
				printf("(%d, ", buf->value);
			else if(buf->code == ABS_Y)
				printf("%d):", buf->value);
			else if(buf->code == ABS_PRESSURE)
				printf("%d\n", buf->value);
		}
	}

