#ifndef ATTENDANCEREGISTER_H
#define ATTENDANCEREGISTER_H

#include <string>
#include <vector>
#include "AttendanceRecord.h"

using namespace std;

class AttendanceRegister
{
private:
    string registerID;
    vector<AttendanceRecord> records;

public:
    AttendanceRegister(const string& registerID);

    void addRecord(const AttendanceRecord& record);

    double calculatePercentage(const string& studentID) const;

    void viewRecords() const;
};

#endif