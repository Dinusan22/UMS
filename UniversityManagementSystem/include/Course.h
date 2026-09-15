
#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

class Course {
private:
    std::string courseCode;
    std::string courseName;
    int capacity;
    std::vector<std::string> enrolledStudents;

public:
    Course(const std::string& code,
           const std::string& name,
           int maxCapacity);

    virtual ~Course() = default;

    bool enrollStudent(const std::string& studentID);

    bool removeStudent(const std::string& studentID);

    bool isFull() const;

    int getEnrolledCount() const;

    std::string getCourseCode() const;

    std::string getCourseName() const;

    virtual int calculateCredits() const = 0;

    virtual void displayCourseDetails() const;
};

#endif