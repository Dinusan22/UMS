#ifndef DUPLICATE_ATTENDANCE_EXCEPTION_H
#define DUPLICATE_ATTENDANCE_EXCEPTION_H

#include "AppException.h"

class DuplicateAttendanceException : public AppException
{
public:
    DuplicateAttendanceException();
};

#endif
