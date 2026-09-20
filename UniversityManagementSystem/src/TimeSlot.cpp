#include "../include/TimeSlot.h"

TimeSlot::TimeSlot(const string& day,
                   const string& startTime,
                   const string& endTime,
                   const string& location)
    : day(day),
      startTime(startTime),
      endTime(endTime),
      location(location)
{
}

bool TimeSlot::overlaps(const TimeSlot& other) const
{
    // Slots on different days cannot clash
    if (day != other.day)
    {
        return false;
    }

    // Convert HH:MM into minutes
    int start1 = stoi(startTime.substr(0, 2)) * 60
               + stoi(startTime.substr(3, 2));

    int end1 = stoi(endTime.substr(0, 2)) * 60
             + stoi(endTime.substr(3, 2));

    int start2 = stoi(other.startTime.substr(0, 2)) * 60
               + stoi(other.startTime.substr(3, 2));

    int end2 = stoi(other.endTime.substr(0, 2)) * 60
             + stoi(other.endTime.substr(3, 2));

    // Check whether the two time intervals overlap
    return start1 < end2 && start2 < end1;
}

bool TimeSlot::operator==(const TimeSlot& other) const
{
    return day == other.day &&
           startTime == other.startTime &&
           endTime == other.endTime &&
           location == other.location;
}