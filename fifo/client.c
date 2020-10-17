#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO1 "./clientfifo"
#define FIFO2 "./serverfifo"

int main(void)
{
	int fd1,fd2;
	char buf[128];

	if(mkfifo(FIFO1, 0666))
	{
		 perror("clientfifo1 error");
	}

	if(mkfifo(FIFO2, 0666))
	{
		 perror("clientfifo2 error");
	}
/************client write part*********************/
	printf("open for writing... \n");
	fd1=open(FIFO1,O_WRONLY);//阻塞
	fd2=open(FIFO1,O_RDONLY);//阻塞
	//fd=open(FIFO,O_RDONLY|O_NONBLOCK);//非阻塞
	printf("opened ... \n");
	if(fd1<0)
	{
		perror("Failed to open fifo:");
		return -1;
	}
	while(1)
	{
		fgets(buf,128,stdin);
		write(fd1,buf,strlen(buf));
		if(strncmp(buf,"exit",4)==0)
		{
			break;
		}
	}
	
/************client read part*********************/	 
	printf("open for reading... \n");
	//fd2=open(FIFO1,O_RDONLY);//阻塞
	//fd=open(FIFO,O_RDONLY|O_NONBLOCK);//非阻塞
	printf("opened ... \n");
	if(fd2<0)
	{
		perror("Failed to open fifo:");
		return -1;
	}
	while(1)
	{
		int count;
		count=read(fd2,buf,127);
		if(count>0)
		{
			buf[count]=0;
			printf("fifoclient receive a string:%s\n",buf);
		}
		if(strncmp(buf,"exit",4)==0)
		{
			break;
		}
	}
	
	close(fd1);
	close(fd2);	
	return 0;
}
