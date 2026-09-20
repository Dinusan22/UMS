#include "../include/SessionCodeCapture.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

SessionCodeCapture::SessionCodeCapture()
    : sessionCode(""),
      active(false)
{
}

SessionCodeCapture::SessionCodeCapture(const string& code)
    : sessionCode(code),
      active(false)
{
}

void SessionCodeCapture::beginSession()
{
    if (sessionCode.empty())
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        int codeNumber = 100000 + rand() % 900000;
        sessionCode = to_string(codeNumber);
    }

    expiryTime = chrono::steady_clock::now() + chrono::seconds(300);
    active = true;

    cout << "Session code attendance started." << endl;
    cout << "Session Code: " << sessionCode << endl;
    cout << "Code expires in 5 minutes." << endl;
}

void SessionCodeCapture::captureNext()
{
    if (!active)
    {
        cout << "Attendance session is not active." << endl;
        return;
    }

    if (chrono::steady_clock::now() > expiryTime)
    {
        cout << "Session code has expired." << endl;
        return;
    }

    string enteredCode;
    cout << "Enter session code: ";
    cin >> enteredCode;

    if (verifyCode(enteredCode))
    {
        cout << "Attendance captured successfully." << endl;
    }
    else
    {
        cout << "Invalid or expired session code." << endl;
    }
}

bool SessionCodeCapture::verifyCode(const string& enteredCode) const
{
    if (!active)
    {
        return false;
    }

    if (chrono::steady_clock::now() > expiryTime)
    {
        return false;
    }

    return enteredCode == sessionCode;
}

void SessionCodeCapture::endSession()
{
    active = false;
    sessionCode.clear();

    cout << "Session code attendance ended." << endl;
}
