#ifndef SESSIONCODECAPTURE_H
#define SESSIONCODECAPTURE_H

#include <chrono>
#include <string>
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
    SessionCodeCapture(const string& code);

    void beginSession() override;
    void captureNext() override;
    void endSession() override;

    bool verifyCode(const string& enteredCode) const override;
};

#endif
