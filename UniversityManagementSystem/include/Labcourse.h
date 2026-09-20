#ifndef LAB_COURSE_H
#define LAB_COURSE_H

#include "Course.h"

using namespace std;

class LabCourse : public Course {
public:
    LabCourse(const string& code,
              const string& name,
              int capacity);

    int calculateCredits() const override;
};

#endif