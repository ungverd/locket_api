#ifndef LOCKET_API_LOGGER_WRAPPER_H
#define LOCKET_API_LOGGER_WRAPPER_H

#include "embedded/kl_lib/uart.h"
#include "api/logger.h"

class LoggerWrapper: public Logger {
public:
    LoggerWrapper(CmdUart_t* uart): uart(uart) {}
    void log(const char *format, ...) override;

private:
    CmdUart_t* uart;
};


#endif //LOCKET_API_LOGGER_WRAPPER_H
