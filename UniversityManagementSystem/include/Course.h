#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <iostream>

#include "Timetable.h"

using namespace std;

class Student;

class Course
{
private:
    string courseCode;
    string courseName;
    int capacity;

    vector<Student*> enrolledStudents;

    Course* prerequisite;

    Timetable timetable;

public:
    Course(const string& code,
           const string& name,
           int maxCapacity);

    virtual ~Course() = default;

    bool enrollStudent(Student* student);
    bool removeStudent(Student* student);

    bool isFull() const;
    int getEnrolledCount() const;

    string getCourseCode() const;
    string getCourseName() const;

    void setPrerequisite(Course* course);
    Course* getPrerequisite() const;

    Timetable& getTimetable();
    const Timetable& getTimetable() const;

    virtual int calculateCredits() const = 0;

    virtual void displayCourseDetails() const;

    friend ostream& operator<<(ostream& out, const Course& course);
};

#endif