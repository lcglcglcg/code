#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#define	usleep(sec) Sleep(sec)
#endif

#define	BUFF_SIZE 4096

int main(int argc ,char *argv[])
{

    char ch = 0;
    FILE *fp = 0;
    long pos = 0;
    char *buffer = NULL;
    char *buffer_temp = NULL;

    buffer_temp = buffer = malloc(BUFF_SIZE);
    if (buffer == NULL) {

	fprintf(stdout, "malloc error!\n");
	return -1;
    }

    for(;;){

	usleep(1000);
	if ((fp = fopen("./log", "r"))) {

	    fseek(fp, pos, SEEK_SET);
	    while((ch = fgetc(fp)) != EOF) {

		switch(ch){

		    case '\r':
		    case '\n':

			if (buffer_temp != buffer) {

			    fprintf(stdout, "%s", buffer);
			    memset(buffer, 0, BUFF_SIZE);
			    buffer_temp = buffer;
			    pos = ftell(fp);
			}

		    default:
			*buffer_temp++ = ch;
		}
	    }

	    fclose(fp);
	}
    }

    return 0;
}

