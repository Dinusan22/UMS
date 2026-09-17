#include "Person.h"

using namespace std;

Person::Person(const string& id,
               const string& name,
               const string& username,
               const string& password)
    : ID(id),
      name(name),
      username(username),
      password(password) {
}

bool Person::login(const string& inputUsername,
                   const string& inputPassword) const {

    return inputUsername == username &&
           inputPassword == password;
}

void Person::logout() const {
    // Logout behavior
}