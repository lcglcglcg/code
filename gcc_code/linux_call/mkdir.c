
#include <stdio.h>
#include <dirent.h>

int main()
{

    DIR *dir = opendir("./php-demo/");
    if (!dir) return -1;

    struct dirent *p = NULL;
    while((p = readdir(dir))) {

	fprintf(stdout, "[name=%s][type=%d]\n", p->d_name, p->d_type);
    }

    return 0;
}


