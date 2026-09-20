#ifndef SESSIONCODECAPTURE_H
#define SESSIONCODECAPTURE_H

#include <string>
#include <chrono>
#include "AttendanceCapture.h"

using namespace std;

class SessionCodeCapture : public AttendanceCapture
{
private:
    string sessionCode;
    chrono::steady_clock::time_point expiryTime;
    bool active;

public:
    SessionCodeCapture();

    void beginSession() override;
    void captureNext() override;
    void endSession() override;
};

#endif