
#include "LectureCourse.h"

LectureCourse::LectureCourse(const std::string& code,
                             const std::string& name,
                             int capacity)
    : Course(code, name, capacity) {
}

int LectureCourse::calculateCredits() const {

    return 3;
}