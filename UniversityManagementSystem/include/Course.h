#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

using namespace std;

class Course {
private:
    string courseCode;
    string courseName;
    int capacity;
    vector<string> enrolledStudents;

public:
    Course(const string& code,
           const string& name,
           int maxCapacity);

    virtual ~Course() = default;

    bool enrollStudent(const string& studentID);

    bool removeStudent(const string& studentID);

    bool isFull() const;

    int getEnrolledCount() const;

    string getCourseCode() const;

    string getCourseName() const;

    virtual int calculateCredits() const = 0;

    virtual void displayCourseDetails() const;
};

#endif