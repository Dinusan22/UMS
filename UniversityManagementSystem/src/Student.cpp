#include "Student.h"
#include "Course.h"

#include <iostream>

using namespace std;


Student::Student(const string& id,
                 const string& name,
                 const string& username,
                 const string& password)
    : Person(id, name, username, password) {
}


void Student::displayMenu() const {

    cout << "\n===== STUDENT MENU =====\n";
    cout << "1. Register Course\n";
    cout << "2. Drop Course\n";
    cout << "3. View Timetable\n";
    cout << "4. View Attendance\n";
    cout << "5. Logout\n";
}


void Student::registerCourse(Course* course) {

    if (course != nullptr) {
        enrolledCourses.push_back(course);
    }
}


void Student::dropCourse(Course* course) {

    for (auto it = enrolledCourses.begin();
         it != enrolledCourses.end();
         ++it) {

        if (*it == course) {

            enrolledCourses.erase(it);

            return;
        }
    }
}


bool Student::isEnrolledIn(Course* course) const {

    for (Course* enrolledCourse : enrolledCourses) {

        if (enrolledCourse == course) {
            return true;
        }
    }

    return false;
}


const vector<Course*>& Student::getEnrolledCourses() const {

    return enrolledCourses;
}


// ============================================================
// Operator <<
// ============================================================

ostream& operator<<(ostream& out, const Student& student) {

    out << "Student ID: "
        << student.getId()
        << ", Name: "
        << student.getName();

    return out;
}