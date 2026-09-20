#include "CourseFullException.h"

CourseFullException::CourseFullException()
    : AppException("Course is full. Enrollment rejected.")
{
}
