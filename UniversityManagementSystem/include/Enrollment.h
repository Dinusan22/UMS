#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include "Student.h"
#include "Course.h"

using namespace std;

class Enrollment {
public:

    bool enrollStudent(Student* student, Course* course);

    bool dropCourse(Student* student, Course* course);

    bool checkPrerequisites(Student* student,
                            Course* course) const;
};

#endif