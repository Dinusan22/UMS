#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <string>

using namespace std;

class TimeSlot
{
private:
    string day;
    string startTime;
    string endTime;
    string location;

public:
    TimeSlot(const string& day,
             const string& startTime,
             const string& endTime,
             const string& location);

    bool overlaps(const TimeSlot& other) const;

    bool operator==(const TimeSlot& other) const;
};

#endif