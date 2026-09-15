
#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"

class Student : public Person {
public:
    Student(const std::string& id,
            const std::string& name,
            const std::string& username,
            const std::string& password);

    void displayMenu() const override;
};

#endif