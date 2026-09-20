#ifndef ATTENDANCECAPTURE_H
#define ATTENDANCECAPTURE_H

class AttendanceCapture
{
public:
    virtual ~AttendanceCapture() = default;

    virtual void beginSession() = 0;
    virtual void captureNext() = 0;
    virtual void endSession() = 0;
};

#endif