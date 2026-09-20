#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <map>

class Student;
class Lecturer;
class Administrator;
class Course;
class AttendanceRecord;
class AttendanceRegister;

class Storage
{
private:
    std::string dataDirectory;

public:
    explicit Storage(const std::string& directory = "Data");

    void saveUsers(const std::vector<Student*>& students,
                   const std::vector<Lecturer*>& lecturers,
                   const std::vector<Administrator*>& administrators,
                   const std::map<std::string, std::vector<std::string>>& enrolments,
                   const std::map<std::string, std::string>& passwords) const;

    void loadUsers(std::vector<Student*>& students,
                   std::vector<Lecturer*>& lecturers,
                   std::vector<Administrator*>& administrators,
                   std::map<std::string, std::vector<std::string>>& enrolments,
                   std::map<std::string, std::string>& passwords) const;

    void saveCourses(const std::vector<Course*>& courses,
                     const std::map<std::string, std::string>& lecturerByCourse,
                     const std::map<std::string, std::vector<std::string>>& courseSlots) const;

    void loadCourses(std::vector<Course*>& courses,
                     std::map<std::string, std::string>& lecturerByCourse,
                     std::map<std::string, std::vector<std::string>>& prerequisiteByCourse,
                     std::map<std::string, std::vector<std::string>>& courseSlots) const;

    void saveAttendance(const std::vector<AttendanceRecord>& records) const;
    void loadAttendance(std::vector<AttendanceRecord>& records) const;

    void saveSessions(const std::map<std::string, std::string>& sessionCourse) const;
    void loadSessions(std::map<std::string, std::string>& sessionCourse) const;
};

#endif
