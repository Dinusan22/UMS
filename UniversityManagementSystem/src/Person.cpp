
#include "Person.h"

Person::Person(const std::string& id,
               const std::string& name,
               const std::string& username,
               const std::string& password)
    : ID(id),
      name(name),
      username(username),
      password(password) {
}

bool Person::login(const std::string& inputUsername,
                   const std::string& inputPassword) const {

    return inputUsername == username &&
           inputPassword == password;
}

void Person::logout() const {
    // Logout behavior will be connected to the UI later.
}