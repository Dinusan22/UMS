#ifndef ATTENDANCECORRECTION_H
#define ATTENDANCECORRECTION_H

#include <string>
#include "AttendanceStatus.h"

using namespace std;

class AttendanceCorrection
{
private:
    string correctionID;
    string correctionTime;
    string lecturerID;
    string reason;
    AttendanceStatus correctedStatus;

public:
    AttendanceCorrection(const string& correctionID,
                          const string& correctionTime,
                          const string& lecturerID,
                          const string& reason,
                          AttendanceStatus correctedStatus);

    void createCorrection();
    void viewCorrection() const;

    string getCorrectionID() const;
    string getCorrectionTime() const;
    string getLecturerID() const;
    string getReason() const;
    AttendanceStatus getCorrectedStatus() const;
};

#endif
