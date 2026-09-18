#include "Enrollment.h"

using namespace std;

bool Enrollment::enrollStudent(
    Course& course,
    string studentID
) {
    if (!checkPrerequisites(studentID, course)) {
    return false;
}

    return course.enrollStudent(studentID);
}

bool Enrollment::dropCourse(
    Course& course,
    string studentID
) {
    return course.removeStudent(studentID);
}

bool Enrollment::checkPrerequisites(
    string studentID,
    const Course& course
) const {
    /*
       Temporary prerequisite checking.

       For now, we only check whether:
       1. Student ID is empty
       2. Course code is empty

       We will improve this later using
       the student's completed courses.
    */

    if (studentID.empty()) {
        return false;
    }

    if (course.getCourseCode().empty()) {
        return false;
    }

    return true;
}