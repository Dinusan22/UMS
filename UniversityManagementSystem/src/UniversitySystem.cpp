#include "../include/UniversitySystem.h"
#include "../include/FileReplayCapture.h"
#include "../include/SessionCodeCapture.h"
#include "../include/AppException.h"
#include "../include/NotEnrolledException.h"
#include "../include/SessionClosedException.h"
#include "../include/DuplicateAttendanceException.h"
#include "../include/AttendanceCorrection.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

namespace {
vector<string> split(const string& line) {
    vector<string> result;
    string field;
    stringstream ss(line);
    while (getline(ss, field, ',')) result.push_back(field);
    return result;
}

string currentDate() {
    time_t now = time(nullptr);
    tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    ostringstream out;
    out << put_time(&local, "%Y-%m-%d");
    return out.str();
}

string currentTime() {
    time_t now = time(nullptr);
    tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    ostringstream out;
    out << put_time(&local, "%H:%M");
    return out.str();
}

string currentDateTime() {
    return currentDate() + " " + currentTime() + ":00";
}
}

UniversitySystem::UniversitySystem()
    : attendanceRegister("MAIN_REGISTER"), storage("Data") {
    loadData();
}

UniversitySystem::~UniversitySystem() {
    if (activeAttendanceSession) {
        activeAttendanceSession->closeSession();
        if (!activeSessionId.empty() && !activeCourseCode.empty())
            sessionCourse[activeSessionId] = activeCourseCode;
        activeAttendanceSession.reset();
    }

    saveData();
    clearData();
}

void UniversitySystem::clearData() {
    for (Student* p : studentRepository.getAll()) delete p;
    for (Lecturer* p : lecturerRepository.getAll()) delete p;
    for (Administrator* p : administratorRepository.getAll()) delete p;
    for (Course* p : courseRepository.getAll()) delete p;

    studentRepository.getAll().clear();
    lecturerRepository.getAll().clear();
    administratorRepository.getAll().clear();
    courseRepository.getAll().clear();
}

string UniversitySystem::readLine(const string& prompt) const {
    cout << prompt;
    string value;
    getline(cin >> ws, value);
    return value;
}

int UniversitySystem::readInt(const string& prompt, int minValue, int maxValue) const {
    while (true) {
        cout << prompt;
        int value;
        if (cin >> value && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid choice. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Student* UniversitySystem::findStudent(const string& id) const {
    return studentRepository.findById(id);
}

Lecturer* UniversitySystem::findLecturer(const string& id) const {
    return lecturerRepository.findById(id);
}

Course* UniversitySystem::findCourse(const string& code) const {
    for (Course* course : courseRepository.getAll())
        if (course && course->getCourseCode() == code) return course;
    return nullptr;
}

bool UniversitySystem::lecturerOwnsCourse(const Lecturer* lecturer,
                                          const Course* course) const {
    if (!lecturer || !course) return false;
    auto it = lecturerByCourse.find(course->getCourseCode());
    return it != lecturerByCourse.end() && it->second == lecturer->getId();
}

string UniversitySystem::makeSessionId() const {
    ostringstream out;
    out << "SES" << setw(4) << setfill('0') << nextSessionNumber;
    return out.str();
}

string UniversitySystem::makeSessionCode() const {
    ostringstream out;
    out << "SC" << setw(4) << setfill('0') << nextSessionNumber;
    return out.str();
}

string UniversitySystem::makeRecordId() const {
    ostringstream out;
    out << "REC" << setw(5) << setfill('0') << nextRecordNumber;
    return out.str();
}

void UniversitySystem::run() {
    while (true) {
        cout << "\n===== UNIVERSITY MANAGEMENT SYSTEM =====\n";
        cout << "1. Login\n2. Exit\n";
        int choice = readInt("Choice: ", 1, 2);
        if (choice == 2) {
            saveData();
            cout << "Data saved. Goodbye!\n";
            return;
        }
        login();
    }
}

void UniversitySystem::login() {
    string username = readLine("Username: ");
    string password = readLine("Password: ");
    Person* user = nullptr;

    for (Student* p : studentRepository.getAll())
        if (p->login(username, password)) { user = p; break; }

    if (!user) {
        for (Lecturer* p : lecturerRepository.getAll())
            if (p->login(username, password)) { user = p; break; }
    }

    if (!user) {
        for (Administrator* p : administratorRepository.getAll())
            if (p->login(username, password)) { user = p; break; }
    }

    if (!user) {
        cout << "Invalid username or password.\n";
        return;
    }

    cout << "\nWelcome, " << user->getName() << "!\n";

    // Only UniversitySystem prints the menus.
    if (auto* student = dynamic_cast<Student*>(user)) studentMenu(student);
    else if (auto* lecturer = dynamic_cast<Lecturer*>(user)) lecturerMenu(lecturer);
    else if (auto* admin = dynamic_cast<Administrator*>(user)) administratorMenu(admin);
}

void UniversitySystem::studentMenu(Student* student) {
    while (true) {
        cout << "\n===== STUDENT MENU =====\n";
        cout << "1. View available courses\n";
        cout << "2. Register course\n";
        cout << "3. Drop course\n";
        cout << "4. View my courses\n";
        cout << "5. View timetable\n";
        cout << "6. View attendance\n";
        cout << "7. Mark attendance\n";
        cout << "8. Logout\n";

        int choice = readInt("Choice: ", 1, 8);
        try {
            if (choice == 1) viewCourses();
            else if (choice == 2) registerCourse(student);
            else if (choice == 3) dropCourse(student);
            else if (choice == 4) {
                cout << "\n===== MY COURSES =====\n";
                if (student->getEnrolledCourses().empty())
                    cout << "No courses enrolled.\n";
                for (Course* course : student->getEnrolledCourses())
                    cout << *course << '\n';
            }
            else if (choice == 5) {
                cout << "\n===== MY TIMETABLE =====\n";
                if (student->getEnrolledCourses().empty()) {
                    cout << "No courses enrolled.\n";
                    continue;
                }

                for (Course* course : student->getEnrolledCourses()) {
                    cout << course->getCourseCode()
                         << " - " << course->getCourseName() << '\n';
                    cout << course->getTimetable();
                    cout << '\n';
                }
            }
            else if (choice == 6) viewStudentAttendance(student);
            else if (choice == 7) markAttendance(student);
            else {
                student->logout();
                return;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}

void UniversitySystem::registerCourse(Student* student) {
    string code = readLine("Course code: ");
    Course* course = findCourse(code);
    if (!course) {
        cout << "Course not found.\n";
        return;
    }

    if (enrollment.enrollStudent(student, course)) {
        cout << "Course registered successfully.\n";
        saveData();
    } else {
        cout << "Course registration failed.\n";
    }
}

void UniversitySystem::dropCourse(Student* student) {
    string code = readLine("Course code: ");
    Course* course = findCourse(code);
    if (!course) { cout << "Course not found.\n"; return; }

    if (enrollment.dropCourse(student, course)) {
        enrolmentsByStudent[student->getId()].erase(
            remove(enrolmentsByStudent[student->getId()].begin(),
                   enrolmentsByStudent[student->getId()].end(), code),
            enrolmentsByStudent[student->getId()].end());
        rebuildStudentTimetable(student);
        saveData();
        cout << "Course dropped successfully.\n";
    } else {
        cout << "Course was not registered.\n";
    }
}

void UniversitySystem::viewStudentAttendance(Student* student) const {
    cout << fixed << setprecision(2)
         << "Attendance percentage: "
         << attendanceRegister.calculatePercentage(student->getId()) << "%\n";
}

void UniversitySystem::lecturerMenu(Lecturer* lecturer) {
    while (true) {
        cout << "\n===== LECTURER MENU =====\n";
        cout << "1. View my courses\n";
        cout << "2. Open attendance session\n";
        cout << "3. Close attendance session\n";
        cout << "4. View attendance report\n";
        cout << "5. Correct attendance\n";
        cout << "6. Logout\n";

        int choice = readInt("Choice: ", 1, 6);
        try {
            if (choice == 1) {
                bool found = false;
                for (Course* c : courseRepository.getAll()) {
                    if (lecturerOwnsCourse(lecturer, c)) {
                        found = true;
                        cout << *c << '\n';
                        cout << "Enrolled: "
                             << c->getEnrolledCount() << '\n';
                    }
                }
                if (!found) cout << "No courses assigned.\n";
            }
            else if (choice == 2) openAttendance(lecturer);
            else if (choice == 3) closeAttendance(lecturer);
            else if (choice == 4) attendanceReport(lecturer);
            else if (choice == 5) correctAttendance(lecturer);
            else {
                lecturer->logout();
                return;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}

void UniversitySystem::openAttendance(Lecturer* lecturer) {
    if (activeAttendanceSession) {
        cout << "An attendance session is already open.\n";
        cout << "Session ID: " << activeSessionId << '\n';
        cout << "Course: " << activeCourseCode << '\n';
        return;
    }

    vector<Course*> courses;
    for (Course* c : courseRepository.getAll())
        if (lecturerOwnsCourse(lecturer, c)) courses.push_back(c);

    if (courses.empty()) {
        cout << "No courses assigned.\n";
        return;
    }

    cout << "\n===== MY COURSES =====\n";
    for (size_t i = 0; i < courses.size(); ++i)
        cout << i + 1 << ". " << courses[i]->getCourseCode()
             << " - " << courses[i]->getCourseName()
             << " | Enrolled: " << courses[i]->getEnrolledCount() << '\n';

    int choice = readInt("Choose course: ", 1,
                         static_cast<int>(courses.size()));
    Course* course = courses[choice - 1];

    if (course->getEnrolledCount() == 0) {
        cout << "No students enrolled.\n";
        return;
    }

    cout << "\nAttendance capture method:\n";
    cout << "1. Session Code (students enter the code)\n";
    cout << "2. File Replay (replay Data/capture.txt)\n";
    int method = readInt("Choose method: ", 1, 2);

    string sessionId = makeSessionId();
    AttendanceCapture* capture = nullptr;

    if (method == 1) {
        capture = new SessionCodeCapture(makeSessionCode());
    } else {
        capture = new FileReplayCapture("Data/capture.txt");
    }

    unique_ptr<AttendanceSession> newSession;

    try {
        newSession = make_unique<AttendanceSession>(
            sessionId,
            currentDate(),
            currentTime(),
            "END",
            capture);

        newSession->openSession();
        ++nextSessionNumber;

        if (method == 1) {
            activeAttendanceSession = move(newSession);
            activeSessionId = sessionId;
            activeCourseCode = course->getCourseCode();
            activeLecturerId = lecturer->getId();

            cout << "\nAttendance session is now OPEN.\n";
            cout << "Students can log in and select Mark attendance.\n";
        } else {
            // File replay is the second capture mechanism required by the project.
            // Each event in the file becomes an AttendanceRecord.
            while (true) {
                newSession->captureAttendance();
                if (!newSession->hasCapturedEvent()) break;

                string studentId = newSession->getCapturedStudentID();
                Student* student = findStudent(studentId);

                if (!student || !student->isEnrolledIn(course)) {
                    cout << "Skipping file event for " << studentId
                         << ": student is not enrolled in "
                         << course->getCourseCode() << ".\n";
                    continue;
                }

                bool duplicate = false;
                for (const AttendanceRecord& record : attendanceRecords) {
                    if (record.getStudentID() == studentId &&
                        record.getSessionID() == sessionId) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    cout << "Skipping duplicate attendance for "
                         << studentId << ".\n";
                    continue;
                }

                AttendanceRecord record(
                    makeRecordId(),
                    studentId,
                    sessionId,
                    currentDateTime(),
                    newSession->getCapturedStatus(),
                    "FileReplayCapture");

                attendanceRegister.addRecord(record);
                attendanceRecords.push_back(record);
                ++nextRecordNumber;
            }

            newSession->closeSession();
            sessionCourse[sessionId] = course->getCourseCode();
            saveData();

            cout << "File replay attendance session " << sessionId
                 << " completed.\n";
        }
    }
    catch (const exception& e) {
        newSession.reset();
        cout << "Attendance error: " << e.what() << '\n';
    }
}

void UniversitySystem::markAttendance(Student* student) {
    if (!student) return;

    if (!activeAttendanceSession) {
        throw SessionClosedException();
    }

    Course* course = findCourse(activeCourseCode);
    if (!course) {
        cout << "Attendance course not found.\n";
        return;
    }

    if (!student->isEnrolledIn(course)) {
        throw NotEnrolledException();
    }

    string enteredCode = readLine("Enter session code: ");

    if (!activeAttendanceSession->verifyCode(enteredCode)) {
        throw SessionClosedException();
    }

    for (const AttendanceRecord& record : attendanceRecords) {
        if (record.getStudentID() == student->getId() &&
            record.getSessionID() == activeSessionId) {
            throw DuplicateAttendanceException();
        }
    }

    AttendanceRecord record(
        makeRecordId(),
        student->getId(),
        activeSessionId,
        currentDateTime(),
        AttendanceStatus::PRESENT,
        "SessionCodeCapture");

    attendanceRegister.addRecord(record);
    attendanceRecords.push_back(record);
    ++nextRecordNumber;

    cout << "Attendance marked successfully for "
         << student->getId() << ".\n";

    saveData();
}

void UniversitySystem::closeAttendance(Lecturer* lecturer) {
    if (!activeAttendanceSession) {
        cout << "There is no active attendance session.\n";
        return;
    }

    if (activeLecturerId != lecturer->getId()) {
        cout << "Only the lecturer who opened this session "
             << "can close it.\n";
        return;
    }

    activeAttendanceSession->closeSession();
    sessionCourse[activeSessionId] = activeCourseCode;

    cout << "Attendance session "
         << activeSessionId
         << " completed.\n";

    activeAttendanceSession.reset();
    activeSessionId.clear();
    activeCourseCode.clear();
    activeLecturerId.clear();

    saveData();
}

void UniversitySystem::attendanceReport(Lecturer* lecturer) const {
    cout << "\n===== ATTENDANCE REPORT =====\n";
    bool found = false;

    for (Course* course : courseRepository.getAll()) {
        if (!lecturerOwnsCourse(lecturer, course)) continue;
        found = true;
        cout << "\nCourse: " << course->getCourseCode()
             << " - " << course->getCourseName() << '\n';

        int sessions = 0;
        for (const auto& s : sessionCourse)
            if (s.second == course->getCourseCode()) ++sessions;

        for (Student* student : studentRepository.getAll()) {
            if (!student->isEnrolledIn(course)) continue;

            double present = 0.0;
            for (const auto& s : sessionCourse) {
                if (s.second != course->getCourseCode()) continue;
                for (const AttendanceRecord& r : attendanceRecords) {
                    if (r.getSessionID() == s.first && r.getStudentID() == student->getId()) {
                        if (r.getEffectiveStatus() == AttendanceStatus::PRESENT)
                            present += 1;
                        else if (r.getEffectiveStatus() == AttendanceStatus::LATE)
                            present += 0.5;
                        break;
                    }
                }
            }

            double percentage = sessions == 0 ? 0.0 : (present * 100.0 / sessions);
            cout << "  " << student->getId() << " - " << student->getName()
                 << " : " << fixed << setprecision(2) << percentage << "%\n";
        }
    }

    if (!found) cout << "No courses assigned to this lecturer.\n";
}

void UniversitySystem::correctAttendance(Lecturer* lecturer)
{
    if (!lecturer)
        return;

    vector<Course*> ownedCourses;
    for (Course* course : courseRepository.getAll())
    {
        if (lecturerOwnsCourse(lecturer, course))
            ownedCourses.push_back(course);
    }

    if (ownedCourses.empty())
    {
        cout << "No courses assigned to this lecturer.\n";
        return;
    }

    cout << "\n===== CORRECT ATTENDANCE =====\n";
    for (size_t i = 0; i < ownedCourses.size(); ++i)
    {
        cout << i + 1 << ". "
             << ownedCourses[i]->getCourseCode()
             << " - " << ownedCourses[i]->getCourseName()
             << '\n';
    }

    int courseChoice = readInt(
        "Choose course: ",
        1,
        static_cast<int>(ownedCourses.size()));

    Course* course = ownedCourses[courseChoice - 1];

    vector<AttendanceRecord*> matchingRecords;
    for (AttendanceRecord& record : attendanceRecords)
    {
        auto sessionIt = sessionCourse.find(record.getSessionID());
        if (sessionIt != sessionCourse.end() &&
            sessionIt->second == course->getCourseCode())
        {
            matchingRecords.push_back(&record);
        }
    }

    if (matchingRecords.empty())
    {
        cout << "No attendance records found for this course.\n";
        return;
    }

    cout << "\nAttendance records:\n";
    for (size_t i = 0; i < matchingRecords.size(); ++i)
    {
        AttendanceRecord* record = matchingRecords[i];
        cout << i + 1 << ". Record: "
             << record->getRecordID()
             << " | Student: " << record->getStudentID()
             << " | Session: " << record->getSessionID()
             << " | Current status: "
             << (record->getEffectiveStatus() == AttendanceStatus::PRESENT
                     ? "PRESENT"
                     : "LATE")
             << '\n';
    }

    int recordChoice = readInt(
        "Choose record: ",
        1,
        static_cast<int>(matchingRecords.size()));

    AttendanceRecord* record = matchingRecords[recordChoice - 1];

    cout << "\nNew attendance status:\n";
    cout << "1. PRESENT\n";
    cout << "2. LATE\n";

    int statusChoice = readInt("Choose status: ", 1, 2);
    AttendanceStatus correctedStatus =
        statusChoice == 1
            ? AttendanceStatus::PRESENT
            : AttendanceStatus::LATE;

    string reason = readLine("Reason for correction: ");
    if (reason.empty())
    {
        cout << "Correction cancelled: reason is required.\n";
        return;
    }

    ostringstream correctionID;
    correctionID << "COR"
                 << setw(5)
                 << setfill('0')
                 << nextCorrectionNumber++;

    AttendanceCorrection correction(
        correctionID.str(),
        currentDateTime(),
        lecturer->getId(),
        reason,
        correctedStatus);

    record->addCorrection(correction);
    attendanceRegister.applyCorrection(
        record->getRecordID(),
        correction);

    cout << "Attendance correction appended successfully.\n";
    cout << "Original record was not edited or deleted.\n";

    saveData();
}

void UniversitySystem::administratorMenu(Administrator* admin) {
    while (true) {
        cout << "\n===== ADMINISTRATOR MENU =====\n";
        cout << "1. Add student\n2. Update student\n3. Remove student\n"
             << "4. Add lecturer\n5. Update lecturer\n6. Create course\n"
             << "7. Edit course\n8. Remove course\n9. View students\n"
             << "10. View lecturers\n11. View courses\n12. Enrollment report\n"
             << "13. Save data\n14. Logout\n";

        int choice = readInt("Choice: ", 1, 14);
        try {
            if (choice == 1) addStudent();
            else if (choice == 2) updateStudent();
            else if (choice == 3) removeStudent();
            else if (choice == 4) addLecturer();
            else if (choice == 5) updateLecturer();
            else if (choice == 6) addCourse();
            else if (choice == 7) editCourse();
            else if (choice == 8) removeCourse();
            else if (choice == 9) viewStudents();
            else if (choice == 10) viewLecturers();
            else if (choice == 11) viewCourses();
            else if (choice == 12) administratorReport();
            else if (choice == 13) saveData();
            else { admin->logout(); return; }
        }
        catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}

void UniversitySystem::addStudent() {
    string id = readLine("Student ID: ");
    string name = readLine("Name: ");
    string username = readLine("Username: ");
    string password = readLine("Password: ");

    if (findStudent(id)) { cout << "Student ID already exists.\n"; return; }
    for (Student* p : studentRepository.getAll()) if (p->getUsername() == username) { cout << "Username already exists.\n"; return; }
    for (Lecturer* p : lecturerRepository.getAll()) if (p->getUsername() == username) { cout << "Username already exists.\n"; return; }
    for (Administrator* p : administratorRepository.getAll()) if (p->getUsername() == username) { cout << "Username already exists.\n"; return; }

    studentRepository.add(new Student(id, name, username, password));
    passwords[username] = password;
    saveData();
    cout << "Student added successfully.\n";
}

void UniversitySystem::updateStudent()
{
    string id = readLine("Student ID to update: ");
    Student* student = findStudent(id);
    if (!student) { cout << "Student not found.\n"; return; }

    string name = readLine("New name: ");
    string username = readLine("New username: ");
    string password = readLine("New password: ");

    student->setName(name);
    student->setUsername(username);
    student->setPassword(password);
    passwords[username] = password;
    saveData();
    cout << "Student updated successfully.\n";
}

void UniversitySystem::addLecturer() {
    string id = readLine("Lecturer ID: ");
    string name = readLine("Name: ");
    string username = readLine("Username: ");
    string password = readLine("Password: ");

    if (findLecturer(id)) { cout << "Lecturer ID already exists.\n"; return; }
    for (Student* p : studentRepository.getAll()) if (p->getUsername() == username) { cout << "Username already exists.\n"; return; }
    for (Lecturer* p : lecturerRepository.getAll()) if (p->getUsername() == username) { cout << "Username already exists.\n"; return; }
    for (Administrator* p : administratorRepository.getAll()) if (p->getUsername() == username) { cout << "Username already exists.\n"; return; }

    lecturerRepository.add(new Lecturer(id, name, username, password));
    passwords[username] = password;
    saveData();
    cout << "Lecturer added successfully.\n";
}

void UniversitySystem::updateLecturer()
{
    string id = readLine("Lecturer ID to update: ");
    Lecturer* lecturer = findLecturer(id);
    if (!lecturer) { cout << "Lecturer not found.\n"; return; }

    string name = readLine("New name: ");
    string username = readLine("New username: ");
    string password = readLine("New password: ");

    lecturer->setName(name);
    lecturer->setUsername(username);
    lecturer->setPassword(password);
    passwords[username] = password;
    saveData();
    cout << "Lecturer updated successfully.\n";
}

void UniversitySystem::editCourse()
{
    string code = readLine("Course code to edit: ");
    Course* course = findCourse(code);
    if (!course) { cout << "Course not found.\n"; return; }

    course->setCourseName(readLine("New course name: "));
    course->setCapacity(readInt("New capacity: ", course->getEnrolledCount(), 10000));

    string lecturerId = readLine("Lecturer ID (or NONE): ");
    if (lecturerId == "NONE") lecturerByCourse.erase(code);
    else if (findLecturer(lecturerId)) lecturerByCourse[code] = lecturerId;
    else cout << "Lecturer not found; previous lecturer retained.\n";

    string prerequisite = readLine("Prerequisite code (or NONE): ");
    if (prerequisite == "NONE") {
        course->setPrerequisite(nullptr);
        prerequisiteByCourse.erase(code);
    } else {
        Course* pre = findCourse(prerequisite);
        if (pre && pre != course) {
            course->setPrerequisite(pre);
            prerequisiteByCourse[code] = {prerequisite};
        } else {
            cout << "Prerequisite not found or invalid; previous prerequisite retained.\n";
        }
    }

    course->getTimetable().clear();
    courseSlots[code].clear();
    string day = readLine("Day: ");
    string start = readLine("Start time (HH:MM): ");
    string end = readLine("End time (HH:MM): ");
    string location = readLine("Location: ");
    TimeSlot slot(day, start, end, location);
    course->getTimetable().addSlot(slot);
    courseSlots[code].push_back(day + "," + start + "," + end + "," + location);

    for (Student* student : studentRepository.getAll())
        rebuildStudentTimetable(student);

    saveData();
    cout << "Course updated successfully.\n";
}

void UniversitySystem::administratorReport() const
{
    cout << "\n===== ENROLMENT SUMMARY REPORT =====\n";
    for (Course* course : courseRepository.getAll())
    {
        cout << course->getCourseCode() << " - " << course->getCourseName()
             << " | Enrolled: " << course->getEnrolledCount()
             << "/" << course->getCapacity() << '\n';
    }
}

void UniversitySystem::addCourse() {
    cout << "\n1. Lecture\n2. Lab\n3. Project\n";
    int type = readInt("Course type: ", 1, 3);
    string code = readLine("Course code: ");
    string name = readLine("Course name: ");
    int capacity = readInt("Capacity: ", 1, 10000);

    if (findCourse(code)) { cout << "Course code already exists.\n"; return; }

    Course* course = nullptr;
    if (type == 1) course = new LectureCourse(code, name, capacity);
    else if (type == 2) course = new LabCourse(code, name, capacity);
    else course = new ProjectCourse(code, name, capacity);
    courseRepository.add(course);

    string pre = readLine("Prerequisite code (or NONE): ");
    if (pre != "NONE" && !pre.empty()) {
        Course* p = findCourse(pre);
        if (p) {
            course->setPrerequisite(p);
            prerequisiteByCourse[code] = {pre};
        } else cout << "Prerequisite not found.\n";
    }

    string lecturerId = readLine("Lecturer ID (or NONE): ");
    if (lecturerId != "NONE" && findLecturer(lecturerId)) lecturerByCourse[code] = lecturerId;
    else if (lecturerId != "NONE") cout << "Lecturer not found.\n";

    string day = readLine("Day: ");
    string start = readLine("Start time (HH:MM): ");
    string end = readLine("End time (HH:MM): ");
    string location = readLine("Location: ");

    course->getTimetable().addSlot(TimeSlot(day, start, end, location));
    courseSlots[code].push_back(day + "," + start + "," + end + "," + location);
    saveData();
    cout << "Course created successfully.\n";
}

void UniversitySystem::removeStudent() {
    string id = readLine("Student ID to remove: ");
    Student* student = findStudent(id);
    if (!student) { cout << "Student not found.\n"; return; }

    for (Course* course : courseRepository.getAll()) course->removeStudent(student);
    studentRepository.remove(student);
    enrolmentsByStudent.erase(id);
    passwords.erase(student->getUsername());
    delete student;
    saveData();
    cout << "Student removed.\n";
}

void UniversitySystem::removeLecturer() {
    string id = readLine("Lecturer ID to remove: ");
    Lecturer* lecturer = findLecturer(id);
    if (!lecturer) { cout << "Lecturer not found.\n"; return; }

    for (auto it = lecturerByCourse.begin(); it != lecturerByCourse.end(); ) {
        if (it->second == id) it = lecturerByCourse.erase(it);
        else ++it;
    }
    lecturerRepository.remove(lecturer);
    passwords.erase(lecturer->getUsername());
    delete lecturer;
    saveData();
    cout << "Lecturer removed.\n";
}

void UniversitySystem::removeCourse() {
    string code = readLine("Course code to remove: ");
    Course* course = findCourse(code);
    if (!course) { cout << "Course not found.\n"; return; }

    for (Student* student : studentRepository.getAll()) {
        course->removeStudent(student);
        student->dropCourse(course);
        rebuildStudentTimetable(student);
    }

    for (Course* other : courseRepository.getAll())
        if (other != course && other->getPrerequisite() == course) other->setPrerequisite(nullptr);

    courseRepository.remove(course);
    lecturerByCourse.erase(code);
    courseSlots.erase(code);
    prerequisiteByCourse.erase(code);
    delete course;
    saveData();
    cout << "Course removed.\n";
}

void UniversitySystem::viewStudents() const {
    cout << "\n===== STUDENTS =====\n";
    if (studentRepository.getAll().empty()) { cout << "No students found.\n"; return; }
    for (Student* p : studentRepository.getAll()) cout << *p << '\n';
}

void UniversitySystem::viewLecturers() const {
    cout << "\n===== LECTURERS =====\n";
    for (Lecturer* p : lecturerRepository.getAll()) cout << p->getId() << " - " << p->getName() << '\n';
}

void UniversitySystem::viewCourses() const {
    cout << "\n===== COURSES =====\n";
    if (courseRepository.getAll().empty()) { cout << "No courses found.\n"; return; }
    for (Course* p : courseRepository.getAll()) cout << *p << '\n';
}

void UniversitySystem::restoreEnrollments() {
    // Reconnect prerequisite pointers after all courses are loaded.
    for (const auto& item : prerequisiteByCourse) {
        Course* course = findCourse(item.first);
        if (course && !item.second.empty()) {
            Course* pre = findCourse(item.second[0]);
            if (pre) course->setPrerequisite(pre);
        }
    }

    for (const auto& item : enrolmentsByStudent) {
        Student* student = findStudent(item.first);
        if (!student) continue;
        for (const string& code : item.second) {
            Course* course = findCourse(code);
            if (!course) continue;
            course->enrollStudent(student);
            student->registerCourse(course);
        }
        rebuildStudentTimetable(student);
    }
}

void UniversitySystem::rebuildStudentTimetable(Student* student) {
    if (!student) return;

    for (const auto& item : courseSlots) {
        for (const string& text : item.second) {
            vector<string> f = split(text);
            if (f.size() == 4)
                student->getTimetable().removeSlot(TimeSlot(f[0], f[1], f[2], f[3]));
        }
    }

    for (Course* course : student->getEnrolledCourses())
        for (const TimeSlot& slot : course->getTimetable().getSlots())
            student->getTimetable().addSlot(slot);
}

void UniversitySystem::saveData() {
    enrolmentsByStudent.clear();
    for (Student* student : studentRepository.getAll())
        for (Course* course : student->getEnrolledCourses())
            enrolmentsByStudent[student->getId()].push_back(course->getCourseCode());

    try {
        storage.saveUsers(studentRepository.getAll(), lecturerRepository.getAll(),
                          administratorRepository.getAll(), enrolmentsByStudent, passwords);
        storage.saveCourses(courseRepository.getAll(), lecturerByCourse, courseSlots);
        storage.saveAttendance(attendanceRecords);
        storage.saveSessions(sessionCourse);
    }
    catch (const exception& e) {
        cout << "Save error: " << e.what() << '\n';
    }
}

void UniversitySystem::loadData() {
    try {
        storage.loadUsers(studentRepository.getAll(), lecturerRepository.getAll(),
                          administratorRepository.getAll(), enrolmentsByStudent, passwords);

        vector<Course*> courses;
        storage.loadCourses(courses, lecturerByCourse, prerequisiteByCourse, courseSlots);
        for (Course* course : courses) courseRepository.add(course);

        for (const auto& item : courseSlots) {
            Course* course = findCourse(item.first);
            if (!course) continue;
            for (const string& text : item.second) {
                vector<string> f = split(text);
                if (f.size() == 4)
                    course->getTimetable().addSlot(TimeSlot(f[0], f[1], f[2], f[3]));
            }
        }

        storage.loadAttendance(attendanceRecords);
        for (const AttendanceRecord& record : attendanceRecords)
            attendanceRegister.addRecord(record);

        storage.loadSessions(sessionCourse);
        restoreEnrollments();

        for (const auto& item : sessionCourse) {
            if (item.first.rfind("SES", 0) == 0) {
                try { nextSessionNumber = max(nextSessionNumber, stoi(item.first.substr(3)) + 1); }
                catch (...) {}
            }
        }

        for (const AttendanceRecord& record : attendanceRecords) {
            if (record.getRecordID().rfind("REC", 0) == 0) {
                try { nextRecordNumber = max(nextRecordNumber, stoi(record.getRecordID().substr(3)) + 1); }
                catch (...) {}
            }

            for (const AttendanceCorrection& correction : record.getCorrections()) {
                if (correction.getCorrectionID().rfind("COR", 0) == 0) {
                    try {
                        nextCorrectionNumber = max(
                            nextCorrectionNumber,
                            stoi(correction.getCorrectionID().substr(3)) + 1);
                    }
                    catch (...) {}
                }
            }
        }
    }
    catch (const exception& e) {
        cout << "Load error: " << e.what() << '\n';
    }
}
