#ifndef LOCKET_API_UART_COMMAND_H
#define LOCKET_API_UART_COMMAND_H

#include <optional>

class UartCommand {
public:
    // Returns true if command name equals expected_name (not case-sensitive).
    virtual bool NameIs(const char* expected_name) const = 0;

    // Reads the next token from the command and tries to interpret it as 32-bit integer value.
    // In case of failure (e.g. no token available, failed to interpret string as a number, etc.)
    // std::nullopt is returned.
    virtual std::optional<int32_t> GetNext() = 0;

    virtual ~UartCommand() = default; // Abstract classes must have virtual destructor.
};


#endif //LOCKET_API_UART_COMMAND_H
