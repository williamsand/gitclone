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
#include "head/kernel_list.h"
#include <pthread.h>
#include <signal.h>
#include "head/motion.h"


/*struct bmp_list 
{	 
	char name[40];	
	struct list_head list;
};*/
extern flag;
int list_show(int lcd_fd)
{

	char fb_buf[800*480*4];
	char bmp_file[10][40];
	int ret,count,i;

	struct bmp_list *node = NULL; //node
	struct list_head *pos = NULL,*n = NULL;	
	struct bmp_list *pnode = NULL; 

	//create list
	struct bmp_list*head = (struct bmp_list*)malloc(sizeof(struct bmp_list));
	if (head == NULL) 
	{		
		printf("file,%s line,%d:malloc error!\n",__FILE__,__LINE__);		
		exit(1);	
	}	
	INIT_LIST_HEAD(&head->list);
	

    	//read bmp_dir and count the amount
    	count = read_dir("/bmp_dir",".bmp",&bmp_file);
	printf(" read bmp file ret = %d \n",count);

	//inset node
	for(i=0;i<count;i++)	
	{		
		node = (struct bmp_list*)malloc(sizeof(struct bmp_list));	
		if (node == NULL) 
		{
			printf("file,%s line,%d:malloc error!\n",__FILE__,__LINE__);			
			exit(1);		
		}	
		strcpy(node->name,bmp_file[i]);			
		list_add_tail(&node->list,&head->list);
	}
	
	//display	
	if (list_empty(&head->list))
	{		
		printf("list is empty!\n");	
	} 
	else 
	{	
		while(1)    //display till flag = 0
		{	
			if(flag == AUTOPLAY)
			{
				list_for_each(pos,&head->list) 
				{			
					pnode = list_entry(pos,struct bmp_list,list);					
					ret = show_24bit_bmp(pnode->name,lcd_fd);
					if(ret != 0)
					{
						printf(" show  %s BMP Failed!\n",pnode->name);
						return -1;
					}
					if(flag != AUTOPLAY)
						break;		
					sleep(2);	
								
				}
			}	
			if(flag == RIGHT)
			{	
				printf("flag=%d\n",flag);
				pos = pos->next;
				if(pos == &head->list)
					pos = pos->next;
				pnode = list_entry(pos,struct bmp_list,list);
				ret = show_24bit_bmp(pnode->name,lcd_fd);
				if(ret != 0)
				{
					printf(" show   %s BMP Failed!\n",pnode->name);
					return -1;
				}
				flag = UNOPRATION;
			}
			
			if(flag == LEFT)
			{	
				printf("flag=%d\n",flag);
				pos = pos->prev;
				if(pos == &head->list)
					pos = pos->prev;
				pnode = list_entry(pos,struct bmp_list,list);
				ret = show_24bit_bmp(pnode->name,lcd_fd);
				if(ret != 0)
				{
					printf(" show   %s BMP Failed!\n",pnode->name);
					return -1;
				}				
				flag = UNOPRATION;
			}
			
			if(flag == STOP)
			{
				printf("flag=%d\n",flag);
				while(1)
				{
					if(pos == &head->list)
						pos = pos->next;
					pnode = list_entry(pos,struct bmp_list,list);
					ret = show_24bit_bmp(pnode->name,lcd_fd);
					if(ret != 0)
					{
						printf(" show   %s BMP Failed!\n",pnode->name);
						return -1;
					}
					if(flag !=STOP)
						break;
				}						
			}
			if(flag == DOWM)
			{
				printf("flag=%d\n",flag);
				pos = pos->next;
				if(pos == &head->list)
						pos = pos->next;
				pnode = list_entry(pos,struct bmp_list,list);
				ret = show_bmp_louver(pnode->name,lcd_fd);
					if(ret != 0)
					{
						printf(" show   %s BMP Failed!\n",pnode->name);
						return -1;
					}	
				flag = UNOPRATION;
			}
		}
		
		
	}
	// free
	if(flag == 5)
		{
	list_for_each_safe(pos,n,&head->list) 
	{	
		list_del(pos);		
		pnode = list_entry(pos,struct bmp_list,list);	
		printf("num %s has removed !\n",pnode->name);		
		free(pnode);
	}	
	free(head);
		}

	
	return 0;
}


