#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>



int  show_24bit_bmp(char *filename,int lcd_fd)
{
	int bmp_fd;
	int ret,x,y,j;
	char fb_buf[800*480*4];
	char bmp_buf[800*480*3];
	pid_t  pid;
		
	// 1  获取24bit BMP 图片文件许可
	bmp_fd = open(filename, O_RDWR);
	if(bmp_fd<0)
		{
			printf(" open %s Failed!\n",filename);
			return -1;
		}	


	//	2 读取24bit BMP 位图数据
	//	跳开头部54字节
	lseek(bmp_fd,54,SEEK_SET);

	ret = read(bmp_fd, bmp_buf, 800*480*3);
	if(ret != 800*480*3)
		{
			printf(" read BMP Failed!\n");
			return -1;
		}


	// 24bit BMP 需要转成 32bit RGB
	// 24bit BMP -> RGB(8:8:8)
	// 32bit RGB -> ARGB(8:8:8:8)
	//bmp_buf  -->	fb_buf	
	for(j=0,y=479;y>=0;y--)
		for(x=0;x<800*4;x+=4,j+=3)
			{			
				fb_buf[y*800*4+x]= bmp_buf[j];
				fb_buf[y*800*4+x+1]= bmp_buf[j+1];
				fb_buf[y*800*4+x+2]= bmp_buf[j+2];
			}


	// 3  将转换后的32bit RGB 数据写入LCD
	// 调整LCD的显示位置从头部开始
	lseek(lcd_fd,0,SEEK_SET);

	ret = write(lcd_fd, fb_buf, 800*480*4);
	if(ret != 800*480*4)
		{
			printf(" write LCD Failed!\n");
			return -1;
		}	

    // 4 释放资源
	close(bmp_fd);	

	return 0;


}

int show_bmp_louver(char *filename,int lcd_fd)
{
	int bmp_fd;
	int ret,x,y,j;
	char fb_buf[800*480*4];
	char bmp_buf[800*480*3];
	pid_t  pid;
			
	bmp_fd = open(filename, O_RDWR);
	if(bmp_fd<0)
		{
			printf(" open %s Failed!\n",filename);
			return -1;
		}
	
	lseek(bmp_fd,54,SEEK_SET);

	ret = read(bmp_fd, bmp_buf, 800*480*3);
	if(ret != 800*480*3)
		{
			printf(" read BMP Failed!\n");
			return -1;
		}
/*************************************************************/	
	for(j=0,y=119;y>=0;y--)
	{
		for(x=0;x<800*4;x+=4,j+=3)
			{			
				fb_buf[y*800*4+x]= bmp_buf[j];
				fb_buf[y*800*4+x+1]= bmp_buf[j+1];
				fb_buf[y*800*4+x+2]= bmp_buf[j+2];
			}	
	ret = write(lcd_fd, fb_buf, 800*4);
	if(ret != 800*4)
		{
			printf(" write LCD Failed!\n");
			return -1;
		}	
	usleep(5000);
	}
/*************************************************************/		
	lseek(bmp_fd,800*120*4,SEEK_SET);	
	for(j=120,y=239;y>=0;y--)
	{
		for(x=0;x<800*4;x+=4,j+=3)
			{			
				fb_buf[y*800*4+x]= bmp_buf[j];
				fb_buf[y*800*4+x+1]= bmp_buf[j+1];
				fb_buf[y*800*4+x+2]= bmp_buf[j+2];
			}	
	ret = write(lcd_fd, fb_buf, 800*4);
	if(ret != 800*4)
		{
			printf(" write LCD Failed!\n");
			return -1;
		}	
	usleep(5000);
	}
/*************************************************************/		
	lseek(bmp_fd,800*240*4,SEEK_SET);	
	for(j=240,y=359;y>=0;y--)
	{
		for(x=0;x<800*4;x+=4,j+=3)
			{			
				fb_buf[y*800*4+x]= bmp_buf[j];
				fb_buf[y*800*4+x+1]= bmp_buf[j+1];
				fb_buf[y*800*4+x+2]= bmp_buf[j+2];
			}	
	ret = write(lcd_fd, fb_buf, 800*4);
	if(ret != 800*4)
		{
			printf(" write LCD Failed!\n");
			return -1;
		}	
	usleep(5000);
	}
	
/*************************************************************/ 

	lseek(bmp_fd,800*360*4,SEEK_SET);
	for(j=360,y=479;y>=0;y--)
	{
		for(x=0;x<800*4;x+=4,j+=3)
			{			
				fb_buf[y*800*4+x]= bmp_buf[j];
				fb_buf[y*800*4+x+1]= bmp_buf[j+1];
				fb_buf[y*800*4+x+2]= bmp_buf[j+2];
			}	
	ret = write(lcd_fd, fb_buf, 800*4);
	if(ret != 800*4)
		{
			printf(" write LCD Failed!\n");
			return -1;
		}	
	usleep(5000);
	}
/*************************************************************/			

    // 4 释放资源
	close(bmp_fd);	

	return 0;
}


