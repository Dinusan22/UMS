#include "PrerequisiteException.h"

PrerequisiteException::PrerequisiteException()
    : AppException("Prerequisite not satisfied.") {
}