#include <cstdio>
#include <cstdarg>
#include "stdout_logger.h"

void StdOutLogger::log(const char* fmt, ...) {
    va_list arguments;
    va_start(arguments, fmt);
    vprintf(fmt, arguments);
    va_end(arguments);
    printf("\n");
}
