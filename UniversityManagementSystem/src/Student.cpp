#include "Student.h"
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