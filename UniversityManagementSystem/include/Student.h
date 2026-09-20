#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include "Timetable.h"

#include <vector>
#include <iostream>

using namespace std;

class Course;

class Student : public Person
{
private:
    vector<Course*> enrolledCourses;

    Timetable timetable;

public:
    Student(const string& id,
            const string& name,
            const string& username,
            const string& password);

    void displayMenu() const override;

    void registerCourse(Course* course);
    void dropCourse(Course* course);

    bool isEnrolledIn(Course* course) const;

    const vector<Course*>& getEnrolledCourses() const;

    Timetable& getTimetable();

    const Timetable& getTimetable() const;

    friend ostream& operator<<(ostream& out, const Student& student);
};

#endif