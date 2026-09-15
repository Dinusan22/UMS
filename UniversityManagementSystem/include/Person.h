
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:
    std::string ID;
    std::string name;
    std::string username;
    std::string password;

public:
    Person(const std::string& id,
           const std::string& name,
           const std::string& username,
           const std::string& password);

    bool login(const std::string& inputUsername,
               const std::string& inputPassword) const;

    void logout() const;

    virtual void displayMenu() const = 0;

    virtual ~Person() = default;
};

#endif