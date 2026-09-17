#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Person.h"

using namespace std;

class Administrator : public Person {
public:
    Administrator(const string& id,
                  const string& name,
                  const string& username,
                  const string& password);

    void displayMenu() const override;
};

#endif