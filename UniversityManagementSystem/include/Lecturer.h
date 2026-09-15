
#ifndef LECTURER_H
#define LECTURER_H

#include "Person.h"

class Lecturer : public Person {
public:
    Lecturer(const std::string& id,
             const std::string& name,
             const std::string& username,
             const std::string& password);

    void displayMenu() const override;
};

#endif