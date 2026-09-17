#include "Lecturer.h"
#include <iostream>

using namespace std;

Lecturer::Lecturer(const string& id,
                   const string& name,
                   const string& username,
                   const string& password)
    : Person(id, name, username, password) {
}

void Lecturer::displayMenu() const {

    cout << "\n===== LECTURER MENU =====\n";
    cout << "1. Open Attendance\n";
    cout << "2. Close Attendance\n";
    cout << "3. View Enrolment\n";
    cout << "4. Attendance Report\n";
    cout << "5. Logout\n";
}