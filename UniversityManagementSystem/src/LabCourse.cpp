#include "LabCourse.h"

using namespace std;

LabCourse::LabCourse(const string& code,
                     const string& name,
                     int capacity)
    : Course(code, name, capacity) {
}

int LabCourse::calculateCredits() const {

    return 2;
}