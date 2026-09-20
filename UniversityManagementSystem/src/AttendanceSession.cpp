#include "AttendanceSession.h"
#include "SessionClosedException.h"
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
    if (status || capture == nullptr)
        return;

    capture->beginSession();
    status = true;
    cout << "Attendance session " << sessionID << " opened." << endl;
}

void AttendanceSession::captureAttendance()
{
    if (!status)
        throw SessionClosedException();

    capture->captureNext();
}

bool AttendanceSession::verifyCode(const string& code) const
{
    if (!status || capture == nullptr)
        return false;
    return capture->verifyCode(code);
}

bool AttendanceSession::hasCapturedEvent() const
{
    return capture != nullptr && capture->hasEvent();
}

string AttendanceSession::getCapturedStudentID() const
{
    return capture != nullptr ? capture->getLastStudentID() : string();
}

AttendanceStatus AttendanceSession::getCapturedStatus() const
{
    return capture != nullptr ? capture->getLastStatus() : AttendanceStatus::PRESENT;
}

void AttendanceSession::closeSession()
{
    if (!status)
        return;

    capture->endSession();
    status = false;
    cout << "Attendance session " << sessionID << " closed." << endl;
}
