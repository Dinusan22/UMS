#include "PrerequisiteException.h"

using namespace std;

PrerequisiteException::PrerequisiteException()
    : AppException("Prerequisite requirement was not completed.") {
}