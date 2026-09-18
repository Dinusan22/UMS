#include "AppException.h"

using namespace std;

AppException::AppException(string message) {
    this->message = message;
}

const char* AppException::what() const noexcept {
    return message.c_str();
}