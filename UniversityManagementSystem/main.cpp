#include <iostream>
#include <string>

#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"

#include "LectureCourse.h"
#include "LabCourse.h"
#include "ProjectCourse.h"

#include "Enrollment.h"

#include "PrerequisiteException.h"
#include "NotEnrolledException.h"
#include "SessionClosedException.h"

using namespace std;


// ============================================================
// Test Course
// ============================================================

void testCourse(Course* course) {

    Student student1(
        "S002",
        "Student One",
        "student1",
        "1234"
    );

    Student student2(
        "S003",
        "Student Two",
        "student2",
        "1234"
    );

    cout << "\n==============================\n";

    course->displayCourseDetails();

    cout << "\nAdding students...\n";


    // --------------------------------------------------------
    // Add Student 1
    // --------------------------------------------------------

    if (course->enrollStudent(&student1)) {

        cout << "S002 enrolled successfully.\n";

    }
    else {

        cout << "S002 could not be enrolled.\n";
    }


    // --------------------------------------------------------
    // Add Student 2
    // --------------------------------------------------------

    if (course->enrollStudent(&student2)) {

        cout << "S003 enrolled successfully.\n";

    }
    else {

        cout << "S003 could not be enrolled.\n";
    }


    // --------------------------------------------------------
    // Test Duplicate Enrollment
    // --------------------------------------------------------

    if (course->enrollStudent(&student1)) {

        cout << "S002 enrolled successfully again.\n";

    }
    else {

        cout << "Duplicate enrolment rejected.\n";
    }


    course->displayCourseDetails();


    // --------------------------------------------------------
    // Remove Student 1
    // --------------------------------------------------------

    cout << "\nRemoving S002...\n";

    if (course->removeStudent(&student1)) {

        cout << "S002 removed successfully.\n";

    }
    else {

        cout << "S002 was not found.\n";
    }


    course->displayCourseDetails();
}


// ============================================================
// Main
// ============================================================

int main() {


    // ========================================================
    // Create Users
    // ========================================================

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


    // ========================================================
    // Static Member Testing
    // ========================================================

    cout << "\n========================================\n";
    cout << "        STATIC MEMBER TESTING\n";
    cout << "========================================\n";

    cout << "Number of Person objects: "
         << Person::getPersonCount()
         << endl;


    // ========================================================
    // Operator << Testing
    // ========================================================

    cout << "\n========================================\n";
    cout << "        OPERATOR << TESTING\n";
    cout << "========================================\n";

    cout << student << endl;


    // ========================================================
    // Polymorphism Test
    // ========================================================

    Person* users[] = {
        &student,
        &lecturer,
        &administrator
    };

    cout << "\n========================================\n";
    cout << "        USER LOGIN TESTING\n";
    cout << "========================================\n";


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

        }
        else {

            cout << "\nInvalid username or password.\n";
        }
    }


    // ========================================================
    // Create Courses
    // ========================================================

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

    cout << "\n========================================\n";
    cout << "        COURSE OPERATOR TESTING\n";
    cout << "========================================\n";

    cout << lecture << endl;
    cout << lab << endl;
    cout << project << endl;


    // ========================================================
    // Create Prerequisite Courses
    // ========================================================

    LectureCourse programming(
        "CO1101",
        "Programming Fundamentals",
        10
    );

    LectureCourse advancedProgramming(
        "CO2206",
        "Advanced Programming",
        10
    );


    // --------------------------------------------------------
    // Set CO1101 as prerequisite for CO2206
    // --------------------------------------------------------

    advancedProgramming.setPrerequisite(&programming);


    // ========================================================
    // Create Enrollment Object
    // ========================================================

    Enrollment enrollment;


    // ========================================================
    // Enrollment Testing
    // ========================================================

    cout << "\n========================================\n";
    cout << "        ENROLLMENT TESTING\n";
    cout << "========================================\n";


    // --------------------------------------------------------
    // Register Student
    // --------------------------------------------------------

    try {

        cout << "\nRegistering student S001 in CO2203...\n";

        if (enrollment.enrollStudent(
                &student,
                &lecture)) {

            cout << "Student registered successfully.\n";

        }
        else {

            cout << "Student registration failed.\n";
        }

    }
    catch (const AppException& error) {

        cout << "Error: "
             << error.what()
             << endl;
    }


    // --------------------------------------------------------
    // Test Duplicate Enrollment
    // --------------------------------------------------------

    cout << "\nTrying to register S001 in CO2203 again...\n";

    try {

        if (enrollment.enrollStudent(
                &student,
                &lecture)) {

            cout << "Student registered successfully again.\n";

        }
        else {

            cout << "Duplicate enrollment rejected.\n";
        }

    }
    catch (const AppException& error) {

        cout << "Error: "
             << error.what()
             << endl;
    }


    // --------------------------------------------------------
    // Drop Course
    // --------------------------------------------------------

    cout << "\nDropping student S001 from CO2203...\n";

    try {

        if (enrollment.dropCourse(
                &student,
                &lecture)) {

            cout << "Student dropped successfully.\n";

        }
        else {

            cout << "Student could not be dropped.\n";
        }

    }
    catch (const AppException& error) {

        cout << "Error: "
             << error.what()
             << endl;
    }


    // --------------------------------------------------------
    // Test Dropping Again
    // --------------------------------------------------------

    cout << "\nTrying to drop CO2203 again...\n";

    try {

        if (enrollment.dropCourse(
                &student,
                &lecture)) {

            cout << "Student dropped successfully.\n";

        }
        else {

            cout << "Student could not be dropped.\n";
        }

    }
    catch (const NotEnrolledException& error) {

        cout << "Not enrolled error: "
             << error.what()
             << endl;
    }


    // ========================================================
    // Prerequisite Testing
    // ========================================================

    cout << "\n========================================\n";
    cout << "        PREREQUISITE TESTING\n";
    cout << "========================================\n";


    // --------------------------------------------------------
    // Try Advanced Programming WITHOUT prerequisite
    // --------------------------------------------------------

    cout << "\nTrying to register CO2206 without CO1101...\n";

    try {

        if (enrollment.enrollStudent(
                &student,
                &advancedProgramming)) {

            cout << "CO2206 registered successfully.\n";

        }
        else {

            cout << "CO2206 registration failed.\n";
        }

    }
    catch (const PrerequisiteException& error) {

        cout << "Prerequisite error: "
             << error.what()
             << endl;
    }


    // --------------------------------------------------------
    // Register prerequisite course
    // --------------------------------------------------------

    cout << "\nRegistering prerequisite CO1101...\n";

    try {

        if (enrollment.enrollStudent(
                &student,
                &programming)) {

            cout << "CO1101 registered successfully.\n";

        }
        else {

            cout << "CO1101 registration failed.\n";
        }

    }
    catch (const AppException& error) {

        cout << "Error: "
             << error.what()
             << endl;
    }


    // --------------------------------------------------------
    // Try Advanced Programming AGAIN
    // --------------------------------------------------------

    cout << "\nTrying to register CO2206 again...\n";

    try {

        if (enrollment.enrollStudent(
                &student,
                &advancedProgramming)) {

            cout << "CO2206 registered successfully.\n";

        }
        else {

            cout << "CO2206 registration failed.\n";
        }

    }
    catch (const PrerequisiteException& error) {

        cout << "Prerequisite error: "
             << error.what()
             << endl;
    }


    // ========================================================
    // Course Testing
    // ========================================================

    cout << "\n========================================\n";
    cout << "        COURSE TESTING\n";
    cout << "========================================\n";


    cout << "\n--- Lecture Course ---\n";

    testCourse(&lecture);


    cout << "\n--- Lab Course ---\n";

    testCourse(&lab);


    cout << "\n--- Project Course ---\n";

    testCourse(&project);


    // ========================================================
    // Exception Testing
    // ========================================================

    cout << "\n========================================\n";
    cout << "        EXCEPTION TESTING\n";
    cout << "========================================\n";


    // --------------------------------------------------------
    // Not Enrolled Exception
    // --------------------------------------------------------

    try {

        cout << "\nTesting NotEnrolledException...\n";

        throw NotEnrolledException();

    }
    catch (const AppException& error) {

        cout << "Caught exception: "
             << error.what()
             << endl;
    }


    // --------------------------------------------------------
    // Session Closed Exception
    // --------------------------------------------------------

    try {

        cout << "\nTesting SessionClosedException...\n";

        throw SessionClosedException();

    }
    catch (const AppException& error) {

        cout << "Caught exception: "
             << error.what()
             << endl;
    }


    // ========================================================
    // Program End
    // ========================================================

    cout << "\n========================================\n";
    cout << "        PROGRAM FINISHED\n";
    cout << "========================================\n";


    return 0;
}