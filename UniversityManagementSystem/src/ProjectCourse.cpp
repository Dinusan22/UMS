#include "ProjectCourse.h"

using namespace std;

ProjectCourse::ProjectCourse(const string& code,
                             const string& name,
                             int capacity)
    : Course(code, name, capacity) {
}

int ProjectCourse::calculateCredits() const {

    return 4;
}