#ifndef LECTURER_H
#define LECTURER_H

#include "Person.h"

using namespace std;

class Lecturer : public Person {
public:
    Lecturer(const string& id,
             const string& name,
             const string& username,
             const string& password);

    void displayMenu() const override;
};

#endif