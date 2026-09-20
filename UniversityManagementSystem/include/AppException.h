#ifndef APP_EXCEPTION_H
#define APP_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class AppException : public exception {
private:
    string message;

public:
    AppException(const string& message);
    const char* what() const noexcept override;
};

#endif