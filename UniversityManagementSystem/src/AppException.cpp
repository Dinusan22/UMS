#include "AppException.h"

AppException::AppException(const string& message)
    : message(message) {
}

const char* AppException::what() const noexcept {
    return message.c_str();
}