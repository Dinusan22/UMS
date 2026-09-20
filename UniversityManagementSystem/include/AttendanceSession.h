#ifndef ATTENDANCESESSION_H
#define ATTENDANCESESSION_H

#include <string>
#include "AttendanceCapture.h"

using namespace std;

class AttendanceSession
{
private:
    string sessionID;
    string date;
    string openTime;
    string closeTime;
    bool status;

    AttendanceCapture* capture;

public:
    AttendanceSession(const string& sessionID,
                      const string& date,
                      const string& openTime,
                      const string& closeTime,
                      AttendanceCapture* capture);

    ~AttendanceSession();

    AttendanceSession(const AttendanceSession&) = delete;
    AttendanceSession& operator=(const AttendanceSession&) = delete;

    void openSession();
    void captureAttendance();
    void closeSession();
};

#endif