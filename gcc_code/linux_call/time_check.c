
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

long int count_success = 0;
long int count_error = 0;

void open_file(char *path)
{

    int size = 0;
    char buffer[1024];
    FILE *fp = NULL;

    if (!(fp = fopen(path, "rb"))) {

	count_error++;
	return;
    }

    while(fgets(buffer, 1024, fp));

    count_success++;
    fclose(fp);
}

void open_dir(char *path)
{

    DIR *dir = NULL;
    struct dirent *dirp;
    char buffer[4096] = {0};

    if((dir = opendir(path)) == NULL) {

	count_error++;
	return;
    }

    while((dirp = readdir(dir))){

	if((strcmp(dirp->d_name,".") == 0)
		|| (strcmp(dirp->d_name,"..") == 0))
	    continue;

	if (dirp->d_type == DT_DIR) {

	    sprintf(buffer, "%s/%s", path, dirp->d_name);
	    open_dir(buffer);

	} else if (dirp->d_type == 8) {

	    sprintf(buffer, "%s/%s", path, dirp->d_name);
	    open_file(buffer);
	}
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{

    if (argc != 2) {

	fprintf(stdout, "arg is error!\n");
	return -1;
    }

    struct timeval time_begin, time_end;
    gettimeofday(&time_begin, NULL);

    open_dir(argv[1]);

    gettimeofday(&time_end, NULL);
    long int usec = (1000 * 1000 * (time_end.tv_sec - time_begin.tv_sec)) + (time_end.tv_usec - time_begin.tv_usec);

    fprintf(stdout, "usec:\t%lus\n", usec);
    fprintf(stdout, "success:\t%d\n", count_success);
    fprintf(stdout, "error:\t%d\n", count_error);

    return 0;
}


