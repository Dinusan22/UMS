
#include "Course.h"

#include <algorithm>
#include <iostream>

Course::Course(const std::string& code,
               const std::string& name,
               int maxCapacity)
    : courseCode(code),
      courseName(name),
      capacity(maxCapacity) {
}

bool Course::enrollStudent(const std::string& studentID) {

    if (isFull()) {
        return false;
    }

    auto studentFound = std::find(
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

bool Course::removeStudent(const std::string& studentID) {

    auto studentFound = std::find(
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
           static_cast<std::size_t>(capacity);
}

int Course::getEnrolledCount() const {

    return static_cast<int>(enrolledStudents.size());
}

std::string Course::getCourseCode() const {

    return courseCode;
}

std::string Course::getCourseName() const {

    return courseName;
}

void Course::displayCourseDetails() const {

    std::cout << "\nCourse Code: "
              << courseCode << '\n';

    std::cout << "Course Name: "
              << courseName << '\n';

    std::cout << "Capacity: "
              << capacity << '\n';

    std::cout << "Enrolled Students: "
              << enrolledStudents.size() << '\n';

    std::cout << "Credits: "
              << calculateCredits() << '\n';
}