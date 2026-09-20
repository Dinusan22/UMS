#ifndef NOT_ENROLLED_EXCEPTION_H
#define NOT_ENROLLED_EXCEPTION_H

#include "AppException.h"

using namespace std;

class NotEnrolledException : public AppException {
public:
    NotEnrolledException();
};

#endif