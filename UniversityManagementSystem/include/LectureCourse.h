#ifndef LECTURE_COURSE_H
#define LECTURE_COURSE_H

#include "Course.h"

using namespace std;

class LectureCourse : public Course {
public:
    LectureCourse(const string& code,
                  const string& name,
                  int capacity);

    int calculateCredits() const override;
};

#endif