#include <stdio.h>
#include <stdarg.h>

void display(char* format, ...)
{
    switch(*format)
    {
	case '1':fprintf(stdout, "\33[0;41m");
		 break;
	case '2':fprintf(stdout, "\33[0;42m");
		 break;
	case '3':fprintf(stdout, "\33[0;43m");
		 break;
	case '4':fprintf(stdout, "\33[0;44m");
		 break;
	case '5':fprintf(stdout, "\33[0;45m");
		 break;
	case '6':fprintf(stdout, "\33[0;46m");
		 break;
	case '7':fprintf(stdout, "\33[0;47m");
		 break;
	default :break;
    }

    char *p;
    va_start(p, format);
    while(*format++)
	fprintf(stdout, va_arg(p, char *));
    fprintf(stdout, "\33[0m\n");
}
#if 0
int main(void)
{
    display("000", "aaa", "bbb", "ccc");
    display("111", "aaa", "bbb", "ccc");
    display("222", "aaa", "bbb", "ccc");
    display("333", "aaa", "bbb", "ccc");
    display("444", "aaa", "bbb", "ccc");
    display("555", "aaa", "bbb", "ccc");
    display("666", "aaa", "bbb", "ccc");
    display("777", "aaa", "bbb", "ccc");
    display("888", "aaa", "bbb", "ccc");
    return 0;
}
#endif
