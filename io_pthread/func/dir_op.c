#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>    
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

// ��dir_name ָ����Ŀ¼�ж�ȡtype���͵��ļ�
// ���ļ�����ŵ�dir_file����
int read_dir(char *dir_name,char *type,char (*dir_file)[10][40])
{
	int i=0,count;
	DIR *_dir;
	struct dirent *_dirent;

	_dir = opendir(dir_name);
	if( _dir == NULL)
		{
			printf(" opendir %s Failed!\n",dir_name);
			return -1;
		}
	
	//create list
	/*struct student *head = (struct student*)malloc(sizeof(struct student));
	if (head == NULL) 
	{		
		printf("file,%s line,%d:malloc error!\n",__FILE__,__LINE__);		
		exit(1);	
	}	
	INIT_LIST_HEAD(&head->list);*/


	

		
	//���ϴӸ�Ŀ¼��ȡͼƬ��Ϣ
	// ֻҪ������ϢΪ�ǿգ���֤�����ļ�����
	while( (_dirent = readdir(_dir)) != NULL )
		{
			// ���ļ����ڣ���������ļ�
			// �жϸ��ļ��Ƿ�Ϊbmpλͼ
			if(strstr(_dirent->d_name,type) != NULL)
				{
					sprintf((*dir_file)[i],"%s/%s",dir_name,_dirent->d_name);
					i++;
				}

		}

	for(count=0;count<i;count++)
		printf(" read  %s file %d : %s \n",dir_name,count,(*dir_file)[count]);

	closedir(_dir);
	return i;
}




/*int  dir_opration( const char *pathname)
{
	DIR  *dir_fd;
	struct dirent  *index_f;
	//struct stat *buf;                               //what is diffrent?
	struct stat *buf = malloc(sizeof(*buf));	
	bzero(buf, sizeof(*buf));

	//open dir
	dir_fd = opendir(pathname);
	if(dir_fd == NULL)
	{
		perror("open dir failed\n");
		exit(1);
	}

	//chdir(pathname);
	//read dir 
	while((index_f = readdir(dir_fd))!= NULL)
	{
		stat(index_f->d_name,buf);
		if(S_ISDIR(buf->st_mode))
		{*/
			/*find dir but ignore . and ..*/
			/*if(strcmp(".",index_f->d_name)==0 || strcmp("..",index_f->d_name)==0)
				continue;
			printf("dirname :%*s%s/\n",0," ",index_f->d_name);	
			//dir_opration( index_f->d_name);
		}
		if(S_ISREG(buf->st_mode))
			printf("filename :%*s%s\n",5," ",index_f->d_name);*/
		
	/*}

	closedir(dir_fd);

	return 0;

}*/






