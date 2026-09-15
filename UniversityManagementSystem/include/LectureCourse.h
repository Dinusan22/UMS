
#ifndef LECTURE_COURSE_H
#define LECTURE_COURSE_H

#include "Course.h"

class LectureCourse : public Course {
public:
    LectureCourse(const std::string& code,
                  const std::string& name,
                  int capacity);

    int calculateCredits() const override;
};

#endif