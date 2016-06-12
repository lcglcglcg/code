
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

    if (!(dir = opendir(path)))
	return -1;

    while((dir_info = readdir(dir))) {

	if((strcmp(dir_info->d_name,".") == 0))
	    continue;

	if ((strcmp(dir_info->d_name,"..")==0))
	    continue;

	fprintf(stdout, "%s/%s\n", path, dir_info->d_name);

	if (directory_check(dir_info->d_name) == 0)
	    directory(dir_info->d_name);
    }

    closedir(dir);
    return 0;
}

int main(int argc,char **argv)
{
    char *path="/make/demo_cpp";
    if(directory_check(path) == 0)
	directory(path);

    return 0;
}




