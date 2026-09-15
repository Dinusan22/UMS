
#include "LabCourse.h"

LabCourse::LabCourse(const std::string& code,
                     const std::string& name,
                     int capacity)
    : Course(code, name, capacity) {
}

int LabCourse::calculateCredits() const {

    return 2;
}