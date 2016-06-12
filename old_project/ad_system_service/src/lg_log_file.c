
#include <stdio.h>
#include <stdarg.h>
#include "lg_log_file.h"

void lg_log(const char *format, ...)
{

    va_list arg_list;
    va_start(arg_list, format);
    vfprintf(stdout, format, arg_list);
}


