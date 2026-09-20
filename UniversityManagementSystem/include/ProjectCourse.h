#ifndef PROJECT_COURSE_H
#define PROJECT_COURSE_H

#include "Course.h"

using namespace std;

class ProjectCourse : public Course {
public:
    ProjectCourse(const string& code,
                  const string& name,
                  int capacity);

    int calculateCredits() const override;
};

#endif