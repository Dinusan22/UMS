#ifndef ATTENDANCECAPTURE_H
#define ATTENDANCECAPTURE_H

#include <string>
#include "AttendanceStatus.h"

class AttendanceCapture
{
public:
    virtual ~AttendanceCapture() = default;

    virtual void beginSession() = 0;
    virtual void captureNext() = 0;
    virtual void endSession() = 0;

    // Optional capabilities exposed through the abstract interface.
    virtual bool verifyCode(const std::string&) const { return false; }
    virtual bool hasEvent() const { return false; }
    virtual const std::string& getLastStudentID() const { static const std::string empty; return empty; }
    virtual AttendanceStatus getLastStatus() const { return AttendanceStatus::PRESENT; }
};

#endif
