#include "../include/AttendanceRegister.h"
#include <iostream>
#include <stdexcept>

using namespace std;

AttendanceRegister::AttendanceRegister(
    const string& registerID)
    : registerID(registerID)
{
}

void AttendanceRegister::addRecord(
    const AttendanceRecord& record)
{
    for (const AttendanceRecord& existingRecord : records)
    {
        if (existingRecord.getStudentID() == record.getStudentID() &&
            existingRecord.getSessionID() == record.getSessionID())
        {
            throw runtime_error(
                "Duplicate attendance record for this student and session."
            );
        }
    }

    records.push_back(record);
}

void AttendanceRegister::applyCorrection(
    const string& recordID,
    const AttendanceCorrection& correction)
{
    for (AttendanceRecord& record : records)
    {
        if (record.getRecordID() == recordID)
        {
            record.addCorrection(correction);
            return;
        }
    }

    throw runtime_error(
        "Attendance record not found: " + recordID
    );
}

double AttendanceRegister::calculatePercentage(
    const string& studentID) const
{
    int total = 0;
    int attendanceMarks = 0;

    for (const AttendanceRecord& record : records)
    {
        if (record.getStudentID() == studentID)
        {
            total++;

            if (record.getEffectiveStatus()
                == AttendanceStatus::PRESENT)
            {
                attendanceMarks += 100;
            }
            else if (record.getEffectiveStatus()
                     == AttendanceStatus::LATE)
            {
                attendanceMarks += 50;
            }
        }
    }

    if (total == 0)
    {
        return 0.0;
    }

    return static_cast<double>(attendanceMarks) / total;
}

void AttendanceRegister::viewRecords() const
{
    cout << "Attendance Register: "
         << registerID << endl;

    cout << "============================"
         << endl;

    if (records.empty())
    {
        cout << "No attendance records found."
             << endl;

        return;
    }

    for (const AttendanceRecord& record : records)
    {
        record.viewRecord();

        cout << "----------------------------"
             << endl;
    }
}