#include "Enrollment.h"

#include "PrerequisiteException.h"
#include "NotEnrolledException.h"

using namespace std;


bool Enrollment::enrollStudent(
    Student* student,
    Course* course
) {

    // Check whether pointers are valid
    if (student == nullptr || course == nullptr) {
        return false;
    }


    // Check prerequisite
    if (!checkPrerequisites(student, course)) {
        throw PrerequisiteException();
    }


    // Check duplicate enrollment
    if (student->isEnrolledIn(course)) {
        return false;
    }


    // Add student to course
    if (!course->enrollStudent(student)) {
        return false;
    }


    // Add course to student's enrolled courses
    student->registerCourse(course);

    return true;
}


bool Enrollment::dropCourse(
    Student* student,
    Course* course
) {

    // Check whether pointers are valid
    if (student == nullptr || course == nullptr) {
        return false;
    }


    // Check whether student is enrolled
    if (!student->isEnrolledIn(course)) {
        throw NotEnrolledException();
    }


    // Remove student from course
    if (!course->removeStudent(student)) {
        return false;
    }


    // Remove course from student
    student->dropCourse(course);

    return true;
}


bool Enrollment::checkPrerequisites(
    Student* student,
    Course* course
) const {

    if (student == nullptr || course == nullptr) {
        return false;
    }


    Course* prerequisite = course->getPrerequisite();


    // No prerequisite
    if (prerequisite == nullptr) {
        return true;
    }


    // Check whether prerequisite is enrolled
    if (student->isEnrolledIn(prerequisite)) {
        return true;
    }


    return false;
}