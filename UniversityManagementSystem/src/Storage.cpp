#include "../include/Storage.h"

#include "../include/Student.h"
#include "../include/Lecturer.h"
#include "../include/Administrator.h"
#include "../include/Course.h"
#include "../include/LectureCourse.h"
#include "../include/Labcourse.h"
#include "../include/ProjectCourse.h"
#include "../include/AttendanceRecord.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

namespace
{
    vector<string> split(const string& line, char delimiter)
    {
        vector<string> fields;
        string field;
        stringstream ss(line);
        while (getline(ss, field, delimiter))
            fields.push_back(field);
        return fields;
    }

    string join(const vector<string>& values, char delimiter)
    {
        string result;
        for (size_t i = 0; i < values.size(); ++i)
        {
            if (i > 0) result += delimiter;
            result += values[i];
        }
        return result;
    }

    string attendanceStatusToString(AttendanceStatus status)
    {
        return status == AttendanceStatus::PRESENT ? "PRESENT" : "LATE";
    }

    AttendanceStatus stringToAttendanceStatus(const string& value)
    {
        if (value == "PRESENT") return AttendanceStatus::PRESENT;
        if (value == "LATE") return AttendanceStatus::LATE;
        throw runtime_error("Invalid attendance status: " + value);
    }
}

Storage::Storage(const string& directory)
    : dataDirectory(directory)
{
    fs::create_directories(dataDirectory);
}

void Storage::saveUsers(
    const vector<Student*>& students,
    const vector<Lecturer*>& lecturers,
    const vector<Administrator*>& administrators,
    const map<string, vector<string>>& enrolments,
    const map<string, string>& passwords) const
{
    ofstream out(dataDirectory + "/users.txt");
    if (!out) throw runtime_error("Unable to write users.txt");

    auto passwordFor = [&](const string& username) {
        auto it = passwords.find(username);
        return it == passwords.end() ? string() : it->second;
    };

    for (const Student* student : students)
    {
        out << "STUDENT|"
            << student->getId() << '|'
            << student->getName() << '|'
            << student->getUsername() << '|'
            << passwordFor(student->getUsername()) << '|';

        auto it = enrolments.find(student->getId());
        if (it != enrolments.end())
            out << join(it->second, ',');
        out << '\n';
    }

    for (const Lecturer* lecturer : lecturers)
    {
        out << "LECTURER|"
            << lecturer->getId() << '|'
            << lecturer->getName() << '|'
            << lecturer->getUsername() << '|'
            << passwordFor(lecturer->getUsername()) << '\n';
    }

    for (const Administrator* admin : administrators)
    {
        out << "ADMIN|"
            << admin->getId() << '|'
            << admin->getName() << '|'
            << admin->getUsername() << '|'
            << passwordFor(admin->getUsername()) << '\n';
    }
}

void Storage::loadUsers(
    vector<Student*>& students,
    vector<Lecturer*>& lecturers,
    vector<Administrator*>& administrators,
    map<string, vector<string>>& enrolments,
    map<string, string>& passwords) const
{
    ifstream in(dataDirectory + "/users.txt");
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        vector<string> f = split(line, '|');

        if (f[0] == "STUDENT" && f.size() >= 5)
        {
            students.push_back(new Student(f[1], f[2], f[3], f[4]));
            passwords[f[3]] = f[4];
            if (f.size() >= 6 && !f[5].empty())
                enrolments[f[1]] = split(f[5], ',');
        }
        else if (f[0] == "LECTURER" && f.size() >= 5)
        {
            lecturers.push_back(new Lecturer(f[1], f[2], f[3], f[4]));
            passwords[f[3]] = f[4];
        }
        else if (f[0] == "ADMIN" && f.size() >= 5)
        {
            administrators.push_back(new Administrator(f[1], f[2], f[3], f[4]));
            passwords[f[3]] = f[4];
        }
    }
}

void Storage::saveCourses(
    const vector<Course*>& courses,
    const map<string, string>& lecturerByCourse,
    const map<string, vector<string>>& courseSlots) const
{
    ofstream out(dataDirectory + "/courses.txt");
    if (!out) throw runtime_error("Unable to write courses.txt");

    for (const Course* course : courses)
    {
        string type = "COURSE";
        if (dynamic_cast<const LectureCourse*>(course)) type = "LECTURE";
        else if (dynamic_cast<const LabCourse*>(course)) type = "LAB";
        else if (dynamic_cast<const ProjectCourse*>(course)) type = "PROJECT";

        string prerequisite = "NONE";
        if (course->getPrerequisite() != nullptr)
            prerequisite = course->getPrerequisite()->getCourseCode();

        string lecturer = "NONE";
        auto lecturerIt = lecturerByCourse.find(course->getCourseCode());
        if (lecturerIt != lecturerByCourse.end()) lecturer = lecturerIt->second;

        string slots;
        auto slotIt = courseSlots.find(course->getCourseCode());
        if (slotIt != courseSlots.end())
            slots = join(slotIt->second, '~');

        out << type << '|'
            << course->getCourseCode() << '|'
            << course->getCourseName() << '|'
            << course->getCapacity() << '|'
            << prerequisite << '|'
            << lecturer << '|'
            << slots << '\n';
    }
}

void Storage::loadCourses(
    vector<Course*>& courses,
    map<string, string>& lecturerByCourse,
    map<string, vector<string>>& prerequisiteByCourse,
    map<string, vector<string>>& courseSlots) const
{
    ifstream in(dataDirectory + "/courses.txt");
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        vector<string> f = split(line, '|');
        if (f.size() < 7) continue;

        Course* course = nullptr;
        if (f[0] == "LECTURE")
            course = new LectureCourse(f[1], f[2], stoi(f[3]));
        else if (f[0] == "LAB")
            course = new LabCourse(f[1], f[2], stoi(f[3]));
        else if (f[0] == "PROJECT")
            course = new ProjectCourse(f[1], f[2], stoi(f[3]));

        if (course == nullptr) continue;

        courses.push_back(course);

        if (f[4] != "NONE" && !f[4].empty())
            prerequisiteByCourse[f[1]].push_back(f[4]);

        if (f[5] != "NONE" && !f[5].empty())
            lecturerByCourse[f[1]] = f[5];

        if (!f[6].empty())
            courseSlots[f[1]] = split(f[6], '~');
    }
}

void Storage::saveAttendance(const vector<AttendanceRecord>& records) const
{
    ofstream out(dataDirectory + "/attendance.txt");
    if (!out) throw runtime_error("Unable to write attendance.txt");

    for (const AttendanceRecord& record : records)
    {
        out << record.getRecordID() << '|'
            << record.getStudentID() << '|'
            << record.getSessionID() << '|'
            << record.getTimestamp() << '|'
            << attendanceStatusToString(record.getEffectiveStatus()) << '|'
            << record.getCaptureMethod() << '\n';
    }
}

void Storage::loadAttendance(vector<AttendanceRecord>& records) const
{
    ifstream in(dataDirectory + "/attendance.txt");
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        vector<string> f = split(line, '|');
        if (f.size() < 6) continue;

        records.emplace_back(
            f[0], f[1], f[2], f[3],
            stringToAttendanceStatus(f[4]), f[5]);
    }
}

void Storage::saveSessions(const map<string, string>& sessionCourse) const
{
    ofstream out(dataDirectory + "/sessions.txt");
    if (!out) throw runtime_error("Unable to write sessions.txt");

    for (const auto& pair : sessionCourse)
        out << pair.first << '|' << pair.second << '\n';
}

void Storage::loadSessions(map<string, string>& sessionCourse) const
{
    ifstream in(dataDirectory + "/sessions.txt");
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        vector<string> f = split(line, '|');
        if (f.size() >= 2)
            sessionCourse[f[0]] = f[1];
    }
}
