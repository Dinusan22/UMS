#include "AttendanceCorrection.h"
#include <iostream>

using namespace std;

AttendanceCorrection::AttendanceCorrection(
    const string& correctionID,
    const string& correctionTime,
    const string& lecturerID,
    const string& reason,
    AttendanceStatus correctedStatus)
    : correctionID(correctionID),
      correctionTime(correctionTime),
      lecturerID(lecturerID),
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
    cout << "Acting Lecturer: " << lecturerID << endl;
    cout << "Reason: " << reason << endl;
    cout << "Corrected Status: "
         << (correctedStatus == AttendanceStatus::PRESENT ? "PRESENT" : "LATE")
         << endl;
}

string AttendanceCorrection::getCorrectionID() const { return correctionID; }
string AttendanceCorrection::getCorrectionTime() const { return correctionTime; }
string AttendanceCorrection::getLecturerID() const { return lecturerID; }
string AttendanceCorrection::getReason() const { return reason; }
AttendanceStatus AttendanceCorrection::getCorrectedStatus() const { return correctedStatus; }
