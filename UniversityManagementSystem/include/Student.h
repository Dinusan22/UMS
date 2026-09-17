#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"

using namespace std;

class Student : public Person {
public:
    Student(const string& id,
            const string& name,
            const string& username,
            const string& password);

    void displayMenu() const override;
};

#endif