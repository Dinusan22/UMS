#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include "Course.h"
#include "PrerequisiteException.h"

#include <string>

using namespace std;

class Enrollment {
public:
    bool enrollStudent(Course& course, string studentID);

    bool dropCourse(Course& course, string studentID);

    bool checkPrerequisites(
        string studentID,
        const Course& course
    ) const;
};

#endif