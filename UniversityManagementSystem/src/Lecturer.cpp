
#include "Lecturer.h"
#include <iostream>

Lecturer::Lecturer(const std::string& id,
                   const std::string& name,
                   const std::string& username,
                   const std::string& password)
    : Person(id, name, username, password) {
}

void Lecturer::displayMenu() const {

    std::cout << "\n===== LECTURER MENU =====\n";
    std::cout << "1. Open Attendance\n";
    std::cout << "2. Close Attendance\n";
    std::cout << "3. View Enrolment\n";
    std::cout << "4. Attendance Report\n";
    std::cout << "5. Logout\n";
}