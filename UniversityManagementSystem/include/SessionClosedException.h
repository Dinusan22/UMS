#ifndef SESSION_CLOSED_EXCEPTION_H
#define SESSION_CLOSED_EXCEPTION_H

#include "AppException.h"

using namespace std;

class SessionClosedException : public AppException {
public:
    SessionClosedException();
};

#endif