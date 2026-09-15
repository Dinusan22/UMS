
#include "Administrator.h"
#include <iostream>

Administrator::Administrator(const std::string& id,
                             const std::string& name,
                             const std::string& username,
                             const std::string& password)
    : Person(id, name, username, password) {
}

void Administrator::displayMenu() const {

    std::cout << "\n===== ADMINISTRATOR MENU =====\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Remove Student\n";
    std::cout << "3. Add Lecturer\n";
    std::cout << "4. Create Course\n";
    std::cout << "5. Remove Course\n";
    std::cout << "6. Generate Report\n";
    std::cout << "7. Logout\n";
}