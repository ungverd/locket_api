#ifndef LOCKET_API_UART_COMMAND_WRAPPER_H
#define LOCKET_API_UART_COMMAND_WRAPPER_H

#include <optional>
#include "kl_lib/shell.h"
#include "api/uart_command.h"

class UartCommandWrapper: public UartCommand {
public:
    UartCommandWrapper(Cmd_t* implementation): implementation(implementation) {}

    bool NameIs(const char* expected_name) const override;
    std::optional<int32_t> GetNext() override;

private:
    Cmd_t* implementation;
};


#endif //LOCKET_API_UART_COMMAND_WRAPPER_H
