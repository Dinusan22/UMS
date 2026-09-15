
#ifndef PROJECT_COURSE_H
#define PROJECT_COURSE_H

#include "Course.h"

class ProjectCourse : public Course {
public:
    ProjectCourse(const std::string& code,
                  const std::string& name,
                  int capacity);

    int calculateCredits() const override;
};

#endif