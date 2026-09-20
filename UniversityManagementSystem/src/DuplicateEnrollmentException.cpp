#include "DuplicateEnrollmentException.h"

DuplicateEnrollmentException::DuplicateEnrollmentException()
    : AppException("Student is already enrolled in this course.")
{
}
