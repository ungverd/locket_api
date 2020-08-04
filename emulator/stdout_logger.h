#ifndef LOCKET_API_STDOUT_LOGGER_H
#define LOCKET_API_STDOUT_LOGGER_H
#include "logger.h"

class StdOutLogger: public Logger {
public:
    void log(const char* fmt, ...) override;
};


#endif //LOCKET_API_STDOUT_LOGGER_H
