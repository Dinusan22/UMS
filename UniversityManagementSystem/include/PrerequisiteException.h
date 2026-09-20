#ifndef PREREQUISITE_EXCEPTION_H
#define PREREQUISITE_EXCEPTION_H

#include "AppException.h"

using namespace std;

class PrerequisiteException : public AppException {
public:
    PrerequisiteException();
};

#endif