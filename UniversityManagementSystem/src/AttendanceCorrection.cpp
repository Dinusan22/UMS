#include "../include/AttendanceCorrection.h"
#include <iostream>

using namespace std;

AttendanceCorrection::AttendanceCorrection(
    const string& correctionID,
    const string& correctionTime,
    const string& reason,
    AttendanceStatus correctedStatus)
    : correctionID(correctionID),
      correctionTime(correctionTime),
      reason(reason),
      correctedStatus(correctedStatus)
{
}

void AttendanceCorrection::createCorrection()
{
    cout << "Correction created successfully." << endl;
}

void AttendanceCorrection::viewCorrection() const
{
    cout << "Correction ID: " << correctionID << endl;
    cout << "Correction Time: " << correctionTime << endl;
    cout << "Reason: " << reason << endl;

    cout << "Corrected Status: ";

    if (correctedStatus == AttendanceStatus::PRESENT)
    {
        cout << "PRESENT";
    }
    else if (correctedStatus == AttendanceStatus::LATE)
    {
        cout << "LATE";
    }

    cout << endl;
}

AttendanceStatus AttendanceCorrection::getCorrectedStatus() const
{
    return correctedStatus;
}