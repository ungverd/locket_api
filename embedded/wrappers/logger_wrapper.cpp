#include <cstdarg>
#include "logger_wrapper.h"

void LoggerWrapper::log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    chSysLock();
    uart->IVsPrintf(format, args);
    uart->PrintEOL();
    chSysUnlock();
}
