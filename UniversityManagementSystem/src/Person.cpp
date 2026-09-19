#include "Person.h"

#include <iostream>

using namespace std;


// Initialize static member
int Person::personCount = 0;


// Constructor
Person::Person(const string& id,
               const string& name,
               const string& username,
               const string& password)
    : id(id),
      name(name),
      username(username),
      password(password) {

    personCount++;
}


// Login
bool Person::login(const string& username,
                   const string& password) {

    return this->username == username &&
           this->password == password;
}


// Logout
void Person::logout() {

    cout << "Logged out successfully." << endl;
}


// Get ID
string Person::getId() const {

    return id;
}


// Get name
string Person::getName() const {

    return name;
}


// Get username
string Person::getUsername() const {

    return username;
}


// Get number of Person objects
int Person::getPersonCount() {

    return personCount;
}