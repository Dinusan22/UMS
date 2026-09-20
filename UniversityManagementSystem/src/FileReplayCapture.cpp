#include "../include/FileReplayCapture.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

FileReplayCapture::FileReplayCapture(const string& filename,
                                     const string& expectedSessionCode)
    : filename(filename),
      expectedSessionCode(expectedSessionCode),
      capturedSessionCode(),
      lastStudentID(),
      lastStatus(AttendanceStatus::PRESENT),
      hasLastEvent(false),
      active(false)
{
}

FileReplayCapture::~FileReplayCapture()
{
    endSession();
}

void FileReplayCapture::beginSession()
{
    if (inputFile.is_open())
        inputFile.close();

    inputFile.open(filename);
    if (!inputFile)
        throw runtime_error("Unable to open attendance file: " + filename);

    string line;
    if (!getline(inputFile, line))
        throw runtime_error("Attendance file is empty.");

    string prefix;
    string fileSessionCode;
    stringstream sessionLine(line);

    if (!getline(sessionLine, prefix, '|') ||
        !getline(sessionLine, fileSessionCode) ||
        prefix != "SESSION" || fileSessionCode.empty())
    {
        throw runtime_error("First line must be SESSION|<session-code>.");
    }

    capturedSessionCode = fileSessionCode;

    if (!expectedSessionCode.empty() && capturedSessionCode != expectedSessionCode)
    {
        throw runtime_error(
            "Session code mismatch. Expected " + expectedSessionCode +
            ", received " + capturedSessionCode + ".");
    }

    active = true;
    hasLastEvent = false;
    lastStudentID.clear();

    cout << "File replay capture started." << endl;
    cout << "Session code verified: " << capturedSessionCode << endl;
}

void FileReplayCapture::captureNext()
{
    if (!active)
        throw runtime_error("Cannot capture attendance. File replay is not active.");

    string line;
    if (!getline(inputFile, line))
    {
        hasLastEvent = false;
        cout << "No more attendance events in file." << endl;
        return;
    }

    if (line.empty())
    {
        hasLastEvent = false;
        return;
    }

    string studentID;
    string statusText;
    stringstream ss(line);

    if (!getline(ss, studentID, '|') ||
        !getline(ss, statusText) ||
        studentID.empty() || statusText.empty())
    {
        throw runtime_error("Malformed attendance record: " + line);
    }

    if (statusText == "PRESENT")
        lastStatus = AttendanceStatus::PRESENT;
    else if (statusText == "LATE")
        lastStatus = AttendanceStatus::LATE;
    else
        throw runtime_error("Invalid attendance status: " + statusText);

    lastStudentID = studentID;
    hasLastEvent = true;

    cout << "Attendance event replayed: Student="
         << lastStudentID << ", Status=" << statusText << endl;
}

void FileReplayCapture::endSession()
{
    active = false;
    hasLastEvent = false;
    if (inputFile.is_open())
        inputFile.close();
}

bool FileReplayCapture::hasEvent() const
{
    return hasLastEvent;
}

const string& FileReplayCapture::getLastStudentID() const
{
    return lastStudentID;
}

AttendanceStatus FileReplayCapture::getLastStatus() const
{
    return lastStatus;
}

const string& FileReplayCapture::getCapturedSessionCode() const
{
    return capturedSessionCode;
}
