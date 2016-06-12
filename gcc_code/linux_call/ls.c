
/*
   1	普通文件
   2	目录文件
   3	字符设备文件
   4	快文件
   5	符号链接
   6	管道文件
   7	SOKET文件
   8	未知文件


   dev_t     st_dev;
   ino_t     st_ino;	
   mode_t    st_mode;    	权限
   nlink_t   st_nlink; 	硬链接计数
   uid_t     st_uid;   	ect/password/用户ID
   gid_t     st_gid;   	/etc/group/组ID
   dev_t     st_rdev;  	忽略
   off_t     st_size;  	文件的大小
   blksize_t st_blksize;	默认4096快的大小
   blkcnt_t  st_blocks; 	占用多少块
   time_t    st_atime;  	访问时间/不改变写时间
   time_t    st_mtime;  	最后修改的时间
   time_t    st_ctime;  	改变INode节点的时间/例如chmos
   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>
#include "bmp_code.h"

void bmp_demo(char *path)
{

    bmp_code bmp;
    if (bmp.open(path) == true) {

	bmp.grey_threshold_sort();
	bmp.binarize();
	bmp.delete_noise();
	bmp.print();
    }
}

int directory_check(char *path)
{
    struct stat buf;
    if(lstat(path,&buf)<0)
	return -1;

    if(S_ISDIR(buf.st_mode))
	return 0;

    return 1;
}

int directory(char *path)
{

    DIR *dir = NULL;
    struct dirent *dir_info = NULL;
    char file_name[4096] = {0};

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	sprintf(file_name, "%s/%s", path, dir_info->d_name);

	if (directory_check(file_name))
	    bmp_demo(file_name);
    }

    closedir(dir);
    return 0;
}

int main(int argc,char **argv)
{

    char path[] ="/make/demo/image";

    if(directory_check(path) == 0)
	directory(path);

    return 0;
}




