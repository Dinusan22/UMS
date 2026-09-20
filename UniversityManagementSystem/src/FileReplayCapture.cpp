#include "../include/FileReplayCapture.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

FileReplayCapture::FileReplayCapture(const string& filename)
    : filename(filename),
      active(false)
{
}

FileReplayCapture::~FileReplayCapture()
{
    if (inputFile.is_open())
    {
        inputFile.close();
    }
}

void FileReplayCapture::beginSession()
{
    inputFile.open(filename);

    if (!inputFile)
    {
        throw runtime_error(
            "Unable to open attendance file: " + filename
        );
    }

    active = true;

    cout << "File replay attendance session started."
         << endl;
}

void FileReplayCapture::captureNext()
{
    if (!active)
    {
        throw runtime_error(
            "Cannot capture attendance. "
            "File replay session is not active."
        );
    }

    string line;

    if (!getline(inputFile, line))
    {
        cout << "No more attendance events in file."
             << endl;

        return;
    }

    string studentID;
    string statusText;

    stringstream ss(line);

    // Check that the line contains two fields
    if (!getline(ss, studentID, ',') ||
        !getline(ss, statusText))
    {
        throw runtime_error(
            "Malformed attendance record: " + line
        );
    }

    // Check for empty fields
    if (studentID.empty() || statusText.empty())
    {
        throw runtime_error(
            "Malformed attendance record: " + line
        );
    }

    // Check that the attendance status is valid
    if (statusText != "PRESENT" &&
        statusText != "LATE")
    {
        throw runtime_error(
            "Invalid attendance status: " + statusText
        );
    }

    cout << "Attendance event replayed: "
         << "Student=" << studentID
         << ", Status=" << statusText
         << endl;
}

void FileReplayCapture::endSession()
{
    active = false;

    if (inputFile.is_open())
    {
        inputFile.close();
    }

    cout << "File replay attendance session ended."
         << endl;
}