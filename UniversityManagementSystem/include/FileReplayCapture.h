#ifndef FILEREPLAYCAPTURE_H
#define FILEREPLAYCAPTURE_H

#include <fstream>
#include <string>
#include "AttendanceCapture.h"
#include "AttendanceStatus.h"

using namespace std;

class FileReplayCapture : public AttendanceCapture
{
private:
    string filename;
    string expectedSessionCode;
    string capturedSessionCode;
    string lastStudentID;
    AttendanceStatus lastStatus;
    bool hasLastEvent;
    bool active;
    ifstream inputFile;

public:
    FileReplayCapture(const string& filename,
                      const string& expectedSessionCode = "");

    ~FileReplayCapture() override;

    void beginSession() override;
    void captureNext() override;
    void endSession() override;

    bool hasEvent() const override;
    const string& getLastStudentID() const override;
    AttendanceStatus getLastStatus() const override;
    const string& getCapturedSessionCode() const;

};

#endif
