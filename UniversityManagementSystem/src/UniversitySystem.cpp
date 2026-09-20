#include "../include/UniversitySystem.h"
#include "../include/FileReplayCapture.h"
#include "../include/AppException.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <set>

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
        cout << "7. Logout\n";

        int choice = readInt("Choice: ", 1, 7);
        try {
            if (choice == 1) viewCourses();
            else if (choice == 2) registerCourse(student);
            else if (choice == 3) dropCourse(student);
            else if (choice == 4) {
                cout << "\n===== MY COURSES =====\n";
                if (student->getEnrolledCourses().empty()) cout << "No courses enrolled.\n";
                for (Course* course : student->getEnrolledCourses()) cout << *course << '\n';
            }
            else if (choice == 5) {
                cout << "\n===== MY TIMETABLE =====\n";
                if (student->getEnrolledCourses().empty()) {
                    cout << "No courses enrolled.\n";
                    continue;
                }
                for (Course* course : student->getEnrolledCourses()) {
                    cout << course->getCourseCode() << " - " << course->getCourseName() << '\n';
                    auto it = courseSlots.find(course->getCourseCode());
                    if (it == courseSlots.end()) {
                        cout << "  No timetable slot.\n";
                        continue;
                    }
                    for (const string& text : it->second) {
                        vector<string> f = split(text);
                        if (f.size() == 4)
                            cout << "  Day: " << f[0] << " | Time: " << f[1]
                                 << "-" << f[2] << " | Venue: " << f[3] << '\n';
                    }
                }
            }
            else if (choice == 6) viewStudentAttendance(student);
            else { student->logout(); return; }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}

void UniversitySystem::registerCourse(Student* student) {
    string code = readLine("Course code: ");
    Course* course = findCourse(code);
    if (!course) { cout << "Course not found.\n"; return; }
    if (student->isEnrolledIn(course)) { cout << "Already enrolled.\n"; return; }
    if (course->isFull()) { cout << "Course is full.\n"; return; }

    // Check every prerequisite in the chain.
    set<string> checked;
    Course* pre = course->getPrerequisite();
    while (pre) {
        if (!checked.insert(pre->getCourseCode()).second) {
            cout << "Prerequisite cycle found. Registration rejected.\n";
            return;
        }
        if (!student->isEnrolledIn(pre)) {
            cout << "Prerequisite not satisfied: " << pre->getCourseCode() << '\n';
            return;
        }
        pre = pre->getPrerequisite();
    }

    // Check the new course against all existing student slots.
    for (const TimeSlot& slot : course->getTimetable().getSlots()) {
        if (student->getTimetable().hasClash(slot)) {
            cout << "Timetable clash detected. Registration rejected.\n";
            return;
        }
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
        cout << "1. View my courses\n2. Open attendance session\n3. View attendance report\n4. Logout\n";
        int choice = readInt("Choice: ", 1, 4);
        try {
            if (choice == 1) {
                bool found = false;
                for (Course* c : courseRepository.getAll()) {
                    if (lecturerOwnsCourse(lecturer, c)) {
                        found = true;
                        cout << *c << '\n';
                        cout << "Enrolled: " << c->getEnrolledCount() << '\n';
                    }
                }
                if (!found) cout << "No courses assigned.\n";
            }
            else if (choice == 2) openAttendance(lecturer);
            else if (choice == 3) attendanceReport(lecturer);
            else { lecturer->logout(); return; }
        }
        catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}

void UniversitySystem::openAttendance(Lecturer* lecturer) {
    vector<Course*> courses;
    for (Course* c : courseRepository.getAll())
        if (lecturerOwnsCourse(lecturer, c)) courses.push_back(c);

    if (courses.empty()) { cout << "No courses assigned.\n"; return; }

    cout << "\n===== MY COURSES =====\n";
    for (size_t i = 0; i < courses.size(); ++i)
        cout << i + 1 << ". " << courses[i]->getCourseCode()
             << " - " << courses[i]->getCourseName()
             << " | Enrolled: " << courses[i]->getEnrolledCount() << '\n';

    int choice = readInt("Choose course: ", 1, static_cast<int>(courses.size()));
    Course* course = courses[choice - 1];
    if (course->getEnrolledCount() == 0) { cout << "No students enrolled.\n"; return; }

    string sessionId = makeSessionId();
    string sessionCode = makeSessionCode();
    string filename = "Data/capture.txt";

    // The first line contains the session code. Other lines are real attendance events.
    vector<string> lines;
    ifstream in(filename);
    string line;
    while (getline(in, line)) lines.push_back(line);
    in.close();

    if (lines.empty()) lines.push_back("SESSION|" + sessionCode);
    else lines[0] = "SESSION|" + sessionCode;

    ofstream out(filename);
    if (!out) { cout << "Cannot open " << filename << "\n"; return; }
    for (const string& s : lines) out << s << '\n';
    out.close();

    FileReplayCapture capture(filename, sessionCode);
    AttendanceSession session(sessionId, currentDate(), currentTime(), "END", &capture);

    try {
        session.openSession();

        while (true) {
            capture.captureNext();
            if (!capture.hasEvent()) break;

            string studentId = capture.getLastStudentID();
            Student* student = findStudent(studentId);

            if (!student) {
                cout << "Student " << studentId << " not found.\n";
                continue;
            }
            if (!student->isEnrolledIn(course)) {
                cout << studentId << " is not enrolled in " << course->getCourseCode() << ".\n";
                continue;
            }

            bool duplicate = false;
            for (const AttendanceRecord& r : attendanceRecords)
                if (r.getStudentID() == studentId && r.getSessionID() == sessionId)
                    duplicate = true;

            if (duplicate) {
                cout << "Attendance already recorded for " << studentId << ".\n";
                continue;
            }

            AttendanceRecord record(makeRecordId(), studentId, sessionId,
                                    currentDateTime(), capture.getLastStatus(),
                                    "FileReplayCapture");
            attendanceRegister.addRecord(record);
            attendanceRecords.push_back(record);
            ++nextRecordNumber;
            cout << "Attendance recorded for " << studentId << ".\n";
        }

        session.closeSession();
        sessionCourse[sessionId] = course->getCourseCode();
        ++nextSessionNumber;
        saveData();
        cout << "Attendance session completed.\n";
    }
    catch (const exception& e) {
        cout << "Attendance error: " << e.what() << '\n';
        session.closeSession();
    }
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

            int present = 0;
            for (const auto& s : sessionCourse) {
                if (s.second != course->getCourseCode()) continue;
                for (const AttendanceRecord& r : attendanceRecords) {
                    if (r.getSessionID() == s.first && r.getStudentID() == student->getId()) {
                        if (r.getEffectiveStatus() == AttendanceStatus::PRESENT) ++present;
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

void UniversitySystem::administratorMenu(Administrator* admin) {
    while (true) {
        cout << "\n===== ADMINISTRATOR MENU =====\n";
        cout << "1. Add student\n2. Remove student\n3. Add lecturer\n4. Create course\n"
             << "5. Remove course\n6. View students\n7. View lecturers\n8. View courses\n"
             << "9. Save data\n10. Logout\n";

        int choice = readInt("Choice: ", 1, 10);
        try {
            if (choice == 1) addStudent();
            else if (choice == 2) removeStudent();
            else if (choice == 3) addLecturer();
            else if (choice == 4) addCourse();
            else if (choice == 5) removeCourse();
            else if (choice == 6) viewStudents();
            else if (choice == 7) viewLecturers();
            else if (choice == 8) viewCourses();
            else if (choice == 9) saveData();
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
        }
    }
    catch (const exception& e) {
        cout << "Load error: " << e.what() << '\n';
    }
}
