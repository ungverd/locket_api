#ifndef LOCKET_API_LOGGER_H
#define LOCKET_API_LOGGER_H

// Interface to output text data to console/UART/...
class Logger {
public:
    // Has same logic as printf function in C:
    // log("hello") will output 'hello'
    // log("hello %d", 5) will output 'hello 5'
    // See full list of %-specifiers here: https://en.cppreference.com/w/cpp/io/c/fprintf
    virtual void log(const char* fmt, ...) = 0;
};

#endif //LOCKET_API_LOGGER_H
