
#include "Student.h"
#include <iostream>

Student::Student(const std::string& id,
                 const std::string& name,
                 const std::string& username,
                 const std::string& password)
    : Person(id, name, username, password) {
}

void Student::displayMenu() const {

    std::cout << "\n===== STUDENT MENU =====\n";
    std::cout << "1. Register Course\n";
    std::cout << "2. Drop Course\n";
    std::cout << "3. View Timetable\n";
    std::cout << "4. View Attendance\n";
    std::cout << "5. Logout\n";
}