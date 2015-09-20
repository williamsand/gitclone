#ifndef __MOTION_H
#define __MOTION_H

#define STOP    	0
#define AUTOPLAY 	1
#define RIGHT   	2
#define UNOPRATION 	3
#define LEFT    	4
#define UP          5
#define DOWM		6
struct bmp_list 
{	 
	char name[40];	
	struct list_head list;
};


#endif



