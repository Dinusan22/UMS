#include "NotEnrolledException.h"

NotEnrolledException::NotEnrolledException()
    : AppException("Student is not enrolled in this course.") {
}