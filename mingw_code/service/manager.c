#include "service.h"
//Remote Manager

void baidu_run(void)
{

    for(;;){

	Sleep(1000);
	MessageBox(NULL, "init", "main", MB_OK);
    }
}

int main(int argc, char **argv)
{

    if (argc > 1) {

	if (strcmp("/add", argv[1]) == 0)
	    return baidu_install("lcglg", NULL, NULL);
	else if (strcmp("/del", argv[1]) == 0)
	    return baidu_uninstall("lcglg");
	return FALSE;
    }

    return baidu_init(TEXT("lcglg"), baidu_run);
}

