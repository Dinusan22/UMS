#include "DuplicateAttendanceException.h"

DuplicateAttendanceException::DuplicateAttendanceException()
    : AppException("Attendance has already been recorded for this student in this session.")
{
}
