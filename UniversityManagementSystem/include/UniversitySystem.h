#ifndef UNIVERSITYSYSTEM_H
#define UNIVERSITYSYSTEM_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Repository.h"
#include "Storage.h"
#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "Course.h"
#include "LectureCourse.h"
#include "LabCourse.h"
#include "ProjectCourse.h"
#include "Enrollment.h"
#include "AttendanceRegister.h"
#include "AttendanceSession.h"
#include "AttendanceRecord.h"

class UniversitySystem
{
private:
    Repository<Student> studentRepository;
    Repository<Lecturer> lecturerRepository;
    Repository<Administrator> administratorRepository;
    Repository<Course> courseRepository;

    Enrollment enrollment;
    AttendanceRegister attendanceRegister;
    Storage storage;

    std::map<std::string, std::string> lecturerByCourse;
    std::map<std::string, std::string> passwords;
    std::map<std::string, std::string> sessionCourse;
    std::map<std::string, std::vector<std::string>> enrolmentsByStudent;
    std::map<std::string, std::vector<std::string>> courseSlots;
    std::map<std::string, std::vector<std::string>> prerequisiteByCourse;
    std::vector<AttendanceRecord> attendanceRecords;

    std::unique_ptr<AttendanceSession> activeAttendanceSession;
    std::string activeSessionId;
    std::string activeCourseCode;
    std::string activeLecturerId;

    int nextSessionNumber = 1;
    int nextRecordNumber = 1;
    int nextCorrectionNumber = 1;

public:
    UniversitySystem();
    ~UniversitySystem();

    void run();
    void login();

    void studentMenu(Student* student);
    void lecturerMenu(Lecturer* lecturer);
    void administratorMenu(Administrator* administrator);

    void addStudent();
    void updateStudent();
    void addLecturer();
    void updateLecturer();
    void addCourse();
    void editCourse();
    void administratorReport() const;
    void removeStudent();
    void removeLecturer();
    void removeCourse();

    void viewStudents() const;
    void viewLecturers() const;
    void viewCourses() const;

    void registerCourse(Student* student);
    void dropCourse(Student* student);
    void viewStudentAttendance(Student* student) const;

    void openAttendance(Lecturer* lecturer);
    void markAttendance(Student* student);
    void closeAttendance(Lecturer* lecturer);
    void attendanceReport(Lecturer* lecturer) const;
    void correctAttendance(Lecturer* lecturer);

    void saveData();
    void loadData();

private:
    Student* findStudent(const std::string& id) const;
    Lecturer* findLecturer(const std::string& id) const;
    Course* findCourse(const std::string& code) const;

    bool lecturerOwnsCourse(const Lecturer* lecturer, const Course* course) const;
    void restoreEnrollments();
    void rebuildStudentTimetable(Student* student);
    void clearData();

    std::string readLine(const std::string& prompt) const;
    int readInt(const std::string& prompt, int minValue, int maxValue) const;
    std::string makeSessionId() const;
    std::string makeSessionCode() const;
    std::string makeRecordId() const;
};

#endif
