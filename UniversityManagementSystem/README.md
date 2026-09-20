# University Management System - Final Integrated Version

## Build (VS Code / PowerShell)
Open the `UniversityManagementSystem` folder in VS Code, then run:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp main.cpp -Iinclude -o university_system.exe
```

Then:

```powershell
.\university_system.exe
```

Keep the `Data` folder beside `main.cpp`. The program saves and reloads users, courses, enrolments, timetable slots, attendance records and attendance sessions.

## Sample login
- Student: `john` / `1234`
- Student: `kamal` / `1234`
- Student: `makee` / `12345`
- Lecturer: `david` / `1234`
- Lecturer: `sajee` / `1234`
- Lecturer: `rajini` / `1234`
- Administrator: `admin` / `admin`

## Course registration rules
- A duplicate course is rejected.
- Every prerequisite in the prerequisite chain must already be enrolled.
- Any overlapping timetable slot is rejected.
- A full course is rejected.

This project does not contain a separate grade/completion entity, so a prerequisite is treated as satisfied when the student has the prerequisite course in their enrolment record.

## Student timetable
The timetable displays **course code, course name, day, time and venue**.

## Attendance File Replay
`Data/capture.txt` uses this format:

```text
SESSION|SC0001
S001|PRESENT
S003|LATE
```

The first line is verified against the current session code. Only students explicitly listed in the file are marked present/late. Other enrolled students are absent for that session and therefore receive 0 marks for that session.

## Attendance correction (append-only)
Lecturers can correct an attendance record from the lecturer menu. A correction stores a unique correction ID, timestamp, acting lecturer ID, reason and corrected status. The original AttendanceRecord is never edited or deleted; corrections are appended and the latest correction determines the effective status. Correction history is persisted in `Data/attendance.txt` and restored on startup.

The selected interactive attendance mechanism is **Option A – Rotating session code**. The system also implements the mandatory `FileReplayCapture`. Both are selected at runtime through the lecturer attendance menu.

## Persistence test
1. Login as admin.
2. Add or remove a user.
3. Exit normally.
4. Recompile and run again.
5. The user change remains because it is stored in `Data/users.txt`.

## Important
The assignment specification requires understanding and explaining the code during the viva. Test the project yourself and make sure every group member understands their part.
