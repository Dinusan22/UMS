
#include "ProjectCourse.h"

ProjectCourse::ProjectCourse(const std::string& code,
                             const std::string& name,
                             int capacity)
    : Course(code, name, capacity) {
}

int ProjectCourse::calculateCredits() const {

    return 4;
}