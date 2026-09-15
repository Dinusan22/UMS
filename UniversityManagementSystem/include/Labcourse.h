
#ifndef LAB_COURSE_H
#define LAB_COURSE_H

#include "Course.h"

class LabCourse : public Course {
public:
    LabCourse(const std::string& code,
              const std::string& name,
              int capacity);

    int calculateCredits() const override;
};

#endif