#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Student;

class Course {
private:
    string courseCode;
    string courseName;
    int capacity;

    vector<Student*> enrolledStudents;

    Course* prerequisite;

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

    virtual int calculateCredits() const = 0;

    virtual void displayCourseDetails() const;

    friend ostream& operator<<(ostream& out, const Course& course);
};

#endif