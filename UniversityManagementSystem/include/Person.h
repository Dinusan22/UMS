#ifndef PERSON_H
#define PERSON_H

#include <string>

using namespace std;

class Person {
protected:
    string ID;
    string name;
    string username;
    string password;

public:
    Person(const string& id,
           const string& name,
           const string& username,
           const string& password);

    bool login(const string& inputUsername,
               const string& inputPassword) const;

    void logout() const;

    virtual void displayMenu() const = 0;

    virtual ~Person() = default;
};

#endif