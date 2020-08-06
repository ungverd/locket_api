#ifndef LOCKET_API_LOGGER_WRAPPER_H
#define LOCKET_API_LOGGER_WRAPPER_H

#include "api/logger.h"

class LoggerWrapper: public Logger {
public:
    void log(const char *fmt, ...) override;
};


#endif //LOCKET_API_LOGGER_WRAPPER_H
