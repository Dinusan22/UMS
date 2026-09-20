#ifndef FILEREPLAYCAPTURE_H
#define FILEREPLAYCAPTURE_H

#include <string>
#include <fstream>
#include "AttendanceCapture.h"

using namespace std;

class FileReplayCapture : public AttendanceCapture
{
private:
    string filename;
    ifstream inputFile;
    bool active;

public:
    FileReplayCapture(const string& filename);

    ~FileReplayCapture();

    void beginSession() override;
    void captureNext() override;
    void endSession() override;
};

#endif