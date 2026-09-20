#include "LectureCourse.h"

using namespace std;

LectureCourse::LectureCourse(const string& code,
                             const string& name,
                             int capacity)
    : Course(code, name, capacity) {
}

int LectureCourse::calculateCredits() const {

    return 3;
}