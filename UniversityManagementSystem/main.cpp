#include <iostream>
#include <memory>
#include <string>

#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "LectureCourse.h"
#include "LabCourse.h"
#include "ProjectCourse.h"
#include "LectureCourse.h"
#include "Enrollment.h"
#include "PrerequisiteException.h"
#include "NotEnrolledException.h"
#include "SessionClosedException.h"

using namespace std;

void testCourse(Course* course) {

    cout << "\n==============================\n";

    course->displayCourseDetails();

    cout << "\nAdding students...\n";

    if (course->enrollStudent("S001")) {
        cout << "S001 enrolled successfully.\n";
    } else {
        cout << "S001 could not be enrolled.\n";
    }

    if (course->enrollStudent("S002")) {
        cout << "S002 enrolled successfully.\n";
    } else {
        cout << "S002 could not be enrolled.\n";
    }

    if (course->enrollStudent("S001")) {
        cout << "S001 enrolled successfully again.\n";
    } else {
        cout << "Duplicate enrolment rejected.\n";
    }

    course->displayCourseDetails();

    cout << "\nRemoving S001...\n";

    if (course->removeStudent("S001")) {
        cout << "S001 removed successfully.\n";
    } else {
        cout << "S001 was not found.\n";
    }

    course->displayCourseDetails();
}

int main() {

    Student student(
        "S001",
        "Sajee",
        "sajee",
        "1234"
    );

    Lecturer lecturer(
        "L001",
        "Mr. Kumar",
        "kumar",
        "5678"
    );

    Administrator administrator(
        "A001",
        "Admin",
        "admin",
        "admin123"
    );

    Person* users[] = {
        &student,
        &lecturer,
        &administrator
    };

    for (Person* user : users) {

        string username;
        string password;

        cout << "\nEnter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        if (user->login(username, password)) {

            cout << "\nLogin successful!\n";

            user->displayMenu();

            user->logout();

        } else {

            cout << "\nInvalid username or password.\n";
        }
    }

    LectureCourse lecture(
        "CO2203",
        "Object Oriented Programming",
        2
    );

     LectureCourse oopCourse(
        "CO2203",
        "Object Oriented Programming",
        2
    );

      Enrollment enrollment;

    try {
        cout << "Registering student S001..." << endl;

        if (enrollment.enrollStudent(oopCourse, "S001")) {
            cout << "Student registered successfully." << endl;
        }
    }
    catch (const AppException& error) {
        cout << "Error: " << error.what() << endl;
    }

    cout << endl;

    try {
        cout << "Testing invalid student ID..." << endl;

        enrollment.enrollStudent(oopCourse, "");
    }
    catch (const AppException& error) {
        cout << "Error: " << error.what() << endl;
    }

    cout << endl;

    cout << "Dropping student S001..." << endl;

    if (enrollment.dropCourse(oopCourse, "S001")) {
        cout << "Student dropped successfully." << endl;
    }

    LabCourse lab(
        "CO2204",
        "Programming Laboratory",
        2
    );

    ProjectCourse project(
        "CO2205",
        "Software Engineering Project",
        2
    );

    testCourse(&lecture);
    testCourse(&lab);
    testCourse(&project);

    cout << "\n--- Exception Testing ---" << endl;
    
// not enrolled exception ,session closedexception
try {
    throw NotEnrolledException();
}
catch (const AppException& e) {
    cout << "Caught exception: " << e.what() << endl;
}

try {
    throw SessionClosedException();
}
catch (const AppException& e) {
    cout << "Caught exception: " << e.what() << endl;
}

    return 0;
}