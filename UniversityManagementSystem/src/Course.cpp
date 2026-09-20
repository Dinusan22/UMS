#include "Course.h"
#include "Student.h"

#include <algorithm>
#include <iostream>

using namespace std;

Course::Course(const string& code,
               const string& name,
               int maxCapacity)
    : courseCode(code),
      courseName(name),
      capacity(maxCapacity),
      prerequisite(nullptr)
{
}

bool Course::enrollStudent(Student* student)
{
    if (student == nullptr)
    {
        return false;
    }

    if (isFull())
    {
        return false;
    }

    auto studentFound = find(
        enrolledStudents.begin(),
        enrolledStudents.end(),
        student
    );

    if (studentFound != enrolledStudents.end())
    {
        return false;
    }

    enrolledStudents.push_back(student);
    return true;
}

bool Course::removeStudent(Student* student)
{
    auto studentFound = find(
        enrolledStudents.begin(),
        enrolledStudents.end(),
        student
    );

    if (studentFound == enrolledStudents.end())
    {
        return false;
    }

    enrolledStudents.erase(studentFound);
    return true;
}

bool Course::isFull() const
{
    return enrolledStudents.size() >=
           static_cast<size_t>(capacity);
}

int Course::getEnrolledCount() const
{
    return static_cast<int>(enrolledStudents.size());
}

int Course::getCapacity() const
{
    return capacity;
}

string Course::getCourseCode() const
{
    return courseCode;
}

string Course::getCourseName() const
{
    return courseName;
}

void Course::setPrerequisite(Course* course)
{
    prerequisite = course;
}

Course* Course::getPrerequisite() const
{
    return prerequisite;
}

Timetable& Course::getTimetable()
{
    return timetable;
}

const Timetable& Course::getTimetable() const
{
    return timetable;
}

void Course::displayCourseDetails() const
{
    cout << "\nCourse Code: "
         << courseCode << '\n';

    cout << "Course Name: "
         << courseName << '\n';

    cout << "Capacity: "
         << capacity << '\n';

    cout << "Enrolled Students: "
         << enrolledStudents.size() << '\n';

    cout << "Credits: "
         << calculateCredits() << '\n';

    if (prerequisite != nullptr)
    {
        cout << "Prerequisite: "
             << prerequisite->getCourseCode()
             << '\n';
    }
    else
    {
        cout << "Prerequisite: None\n";
    }
}

ostream& operator<<(ostream& out, const Course& course)
{
    out << "Course Code: "
        << course.courseCode
        << ", Name: "
        << course.courseName
        << ", Capacity: "
        << course.capacity
        << ", Enrolled: "
        << course.enrolledStudents.size()
        << ", Credits: "
        << course.calculateCredits();

    return out;
}