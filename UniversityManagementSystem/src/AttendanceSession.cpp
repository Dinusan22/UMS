#include "../include/AttendanceSession.h"
#include <iostream>

using namespace std;

AttendanceSession::AttendanceSession(
    const string& sessionID,
    const string& date,
    const string& openTime,
    const string& closeTime,
    AttendanceCapture* capture)
    : sessionID(sessionID),
      date(date),
      openTime(openTime),
      closeTime(closeTime),
      status(false),
      capture(capture)
{
}

AttendanceSession::~AttendanceSession()
{
    delete capture;
}

void AttendanceSession::openSession()
{
    if (!status)
    {
        status = true;

        cout << "Attendance session " << sessionID
             << " opened." << endl;

        capture->beginSession();
    }
}

void AttendanceSession::captureAttendance()
{
    if (status)
    {
        capture->captureNext();
    }
    else
    {
        cout << "Cannot capture attendance. "
             << "The session is closed." << endl;
    }
}

void AttendanceSession::closeSession()
{
    if (status)
    {
        status = false;

        cout << "Attendance session " << sessionID
             << " closed." << endl;

        capture->endSession();
    }
}