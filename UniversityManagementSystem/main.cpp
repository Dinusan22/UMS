
#include <iostream>
#include <memory>
#include <string>

#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "LectureCourse.h"
#include "LabCourse.h"
#include "ProjectCourse.h"

void testCourse(Course* course) {

    std::cout << "\n==============================\n";

    course->displayCourseDetails();

    std::cout << "\nAdding students...\n";

    if (course->enrollStudent("S001")) {
        std::cout << "S001 enrolled successfully.\n";
    } else {
        std::cout << "S001 could not be enrolled.\n";
    }

    if (course->enrollStudent("S002")) {
        std::cout << "S002 enrolled successfully.\n";
    } else {
        std::cout << "S002 could not be enrolled.\n";
    }

    if (course->enrollStudent("S001")) {
        std::cout << "S001 enrolled successfully again.\n";
    } else {
        std::cout << "Duplicate enrolment rejected.\n";
    }

    course->displayCourseDetails();

    std::cout << "\nRemoving S001...\n";

    if (course->removeStudent("S001")) {
        std::cout << "S001 removed successfully.\n";
    } else {
        std::cout << "S001 was not found.\n";
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

        std::string username;
        std::string password;

        std::cout << "\nEnter username: ";
        std::cin >> username;

        std::cout << "Enter password: ";
        std::cin >> password;

        if (user->login(username, password)) {

            std::cout << "\nLogin successful!\n";

            user->displayMenu();

            user->logout();

        } else {

            std::cout << "\nInvalid username or password.\n";
        }
    }

     LectureCourse lecture(
        "CO2203",
        "Object Oriented Programming",
        2
    );

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

    return 0;
}
