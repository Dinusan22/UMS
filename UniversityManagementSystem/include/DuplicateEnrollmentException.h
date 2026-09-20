#ifndef DUPLICATE_ENROLLMENT_EXCEPTION_H
#define DUPLICATE_ENROLLMENT_EXCEPTION_H

#include "AppException.h"

class DuplicateEnrollmentException : public AppException
{
public:
    DuplicateEnrollmentException();
};

#endif
