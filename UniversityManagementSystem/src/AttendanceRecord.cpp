#include "../include/AttendanceRecord.h"
#include <iostream>

using namespace std;

AttendanceRecord::AttendanceRecord(
    const string& recordID,
    const string& studentID,
    const string& sessionID,
    const string& timestamp,
    AttendanceStatus status,
    const string& captureMethod)
    : recordID(recordID),
      studentID(studentID),
      sessionID(sessionID),
      timestamp(timestamp),
      status(status),
      captureMethod(captureMethod)
{
}

void AttendanceRecord::markAttendance(AttendanceStatus newStatus)
{
    status = newStatus;
}

void AttendanceRecord::viewRecord() const
{
    cout << "Record ID: " << recordID << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "Session ID: " << sessionID << endl;
    cout << "Timestamp: " << timestamp << endl;

    cout << "Original Status: ";

    if (status == AttendanceStatus::PRESENT)
    {
        cout << "PRESENT";
    }
    else if (status == AttendanceStatus::LATE)
    {
        cout << "LATE";
    }

    cout << endl;

    cout << "Effective Status: ";

    if (getEffectiveStatus() == AttendanceStatus::PRESENT)
    {
        cout << "PRESENT";
    }
    else if (getEffectiveStatus() == AttendanceStatus::LATE)
    {
        cout << "LATE";
    }

    cout << endl;

    cout << "Capture Method: " << captureMethod << endl;

    if (!corrections.empty())
    {
        cout << "Corrections:" << endl;

        for (const AttendanceCorrection& correction : corrections)
        {
            correction.viewCorrection();
            cout << "------------------------" << endl;
        }
    }
}

string AttendanceRecord::getRecordID() const
{
    return recordID;
}

string AttendanceRecord::getStudentID() const
{
    return studentID;
}

string AttendanceRecord::getSessionID() const
{
    return sessionID;
}

string AttendanceRecord::getTimestamp() const
{
    return timestamp;
}

AttendanceStatus AttendanceRecord::getStatus() const
{
    return status;
}

AttendanceStatus AttendanceRecord::getEffectiveStatus() const
{
    if (corrections.empty())
    {
        return status;
    }

    return corrections.back().getCorrectedStatus();
}

string AttendanceRecord::getCaptureMethod() const
{
    return captureMethod;
}

void AttendanceRecord::addCorrection(
    const AttendanceCorrection& correction)
{
    corrections.push_back(correction);
}