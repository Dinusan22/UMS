#ifndef PERSON_H
#define PERSON_H

#include <string>

using namespace std;

class Person {
private:
    string id;
    string name;
    string username;
    string password;

    // Static counter for Person objects
    static int personCount;

public:
    Person(const string& id,
           const string& name,
           const string& username,
           const string& password);

    virtual ~Person() = default;

    bool login(const string& username,
               const string& password);

    void logout();

    string getId() const;
    string getName() const;
    string getUsername() const;

    // Static function
    static int getPersonCount();

    virtual void displayMenu() const = 0;
};

#endif