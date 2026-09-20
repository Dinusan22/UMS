#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <string>
#include <vector>
#include "AttendanceStatus.h"
#include "AttendanceCorrection.h"

using namespace std;

class AttendanceRecord
{
private:
    string recordID;
    string studentID;
    string sessionID;
    string timestamp;
    AttendanceStatus status;
    string captureMethod;
    vector<AttendanceCorrection> corrections;

public:
    AttendanceRecord(const string& recordID,
                     const string& studentID,
                     const string& sessionID,
                     const string& timestamp,
                     AttendanceStatus status,
                     const string& captureMethod);

    void viewRecord() const;

    string getRecordID() const;
    string getStudentID() const;
    string getSessionID() const;
    string getTimestamp() const;
    AttendanceStatus getStatus() const;
    AttendanceStatus getEffectiveStatus() const;
    string getCaptureMethod() const;

    void addCorrection(const AttendanceCorrection& correction);
    const vector<AttendanceCorrection>& getCorrections() const;
};

#endif
