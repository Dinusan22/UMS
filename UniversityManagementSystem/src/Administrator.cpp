#include "Administrator.h"
#include <iostream>

using namespace std;

Administrator::Administrator(const string& id,
                             const string& name,
                             const string& username,
                             const string& password)
    : Person(id, name, username, password) {
}

void Administrator::displayMenu() const {

    cout << "\n===== ADMINISTRATOR MENU =====\n";
    cout << "1. Add Student\n";
    cout << "2. Remove Student\n";
    cout << "3. Add Lecturer\n";
    cout << "4. Create Course\n";
    cout << "5. Remove Course\n";
    cout << "6. Generate Report\n";
    cout << "7. Logout\n";
}