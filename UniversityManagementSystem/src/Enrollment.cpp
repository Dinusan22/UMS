#include "Enrollment.h"

#include "PrerequisiteException.h"
#include "NotEnrolledException.h"

using namespace std;

bool Enrollment::enrollStudent(
    Student* student,
    Course* course
)
{
    if (student == nullptr || course == nullptr)
    {
        return false;
    }

    // Check prerequisite
    if (!checkPrerequisites(student, course))
    {
        throw PrerequisiteException();
    }

    // Check duplicate enrollment
    if (student->isEnrolledIn(course))
    {
        return false;
    }

    // Check timetable clash
    if (detectClash(student, course))
    {
        return false;
    }

    // Check course capacity and add student
    if (!course->enrollStudent(student))
    {
        return false;
    }

    // Add course to student's enrolled courses
    student->registerCourse(course);

    // Add the course timetable slots
    // to the student's timetable
    const vector<TimeSlot>& courseSlots =
        course->getTimetable().getSlots();

    for (const TimeSlot& slot : courseSlots)
    {
        student->getTimetable().addSlot(slot);
    }

    return true;
}

bool Enrollment::dropCourse(
    Student* student,
    Course* course
)
{
    if (student == nullptr || course == nullptr)
    {
        return false;
    }

    if (!student->isEnrolledIn(course))
    {
        throw NotEnrolledException();
    }

    if (!course->removeStudent(student))
    {
        return false;
    }

    student->dropCourse(course);

    return true;
}

bool Enrollment::checkPrerequisites(
    Student* student,
    Course* course
) const
{
    if (student == nullptr || course == nullptr)
    {
        return false;
    }

    Course* prerequisite = course->getPrerequisite();

    if (prerequisite == nullptr)
    {
        return true;
    }

    if (student->isEnrolledIn(prerequisite))
    {
        return true;
    }

    return false;
}

bool Enrollment::detectClash(
    Student* student,
    Course* course
) const
{
    if (student == nullptr || course == nullptr)
    {
        return false;
    }

    const vector<TimeSlot>& courseSlots =
        course->getTimetable().getSlots();

    for (const TimeSlot& slot : courseSlots)
    {
        if (student->getTimetable().hasClash(slot))
        {
            return true;
        }
    }

    return false;
}