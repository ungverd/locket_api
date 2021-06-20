#include "uart_command_wrapper.h"

bool UartCommandWrapper::NameIs(const char* expected_name) const {
    return implementation->NameIs(expected_name);
}

std::optional<int32_t> UartCommandWrapper::GetNext() {
    uint32_t result = 0;
    if (implementation->GetNext(&result) == retvOk) return result;
    return std::nullopt;
}