#include "Course.h"

#include <algorithm>
#include <iostream>

using namespace std;

Course::Course(const string& code,
               const string& name,
               int maxCapacity)
    : courseCode(code),
      courseName(name),
      capacity(maxCapacity) {
}

bool Course::enrollStudent(const string& studentID) {

    if (isFull()) {
        return false;
    }

    auto studentFound = find(
        enrolledStudents.begin(),
        enrolledStudents.end(),
        studentID
    );

    if (studentFound != enrolledStudents.end()) {
        return false;
    }

    enrolledStudents.push_back(studentID);

    return true;
}

bool Course::removeStudent(const string& studentID) {

    auto studentFound = find(
        enrolledStudents.begin(),
        enrolledStudents.end(),
        studentID
    );

    if (studentFound == enrolledStudents.end()) {
        return false;
    }

    enrolledStudents.erase(studentFound);

    return true;
}

bool Course::isFull() const {

    return enrolledStudents.size() >=
           static_cast<size_t>(capacity);
}

int Course::getEnrolledCount() const {

    return static_cast<int>(enrolledStudents.size());
}

string Course::getCourseCode() const {

    return courseCode;
}

string Course::getCourseName() const {

    return courseName;
}

void Course::displayCourseDetails() const {

    cout << "\nCourse Code: "
         << courseCode << '\n';

    cout << "Course Name: "
         << courseName << '\n';

    cout << "Capacity: "
         << capacity << '\n';

    cout << "Enrolled Students: "
         << enrolledStudents.size() << '\n';

    cout << "Credits: "
         << calculateCredits() << '\n';
}