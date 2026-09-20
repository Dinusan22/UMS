#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <vector>
#include <iostream>
#include "TimeSlot.h"

using namespace std;

class Timetable
{
private:
    vector<TimeSlot> slots;

public:
    void addSlot(const TimeSlot& slot);

    void removeSlot(const TimeSlot& slot);

    bool hasClash(const TimeSlot& slot) const;

    const vector<TimeSlot>& getSlots() const;

    friend ostream& operator<<(ostream& out,
                               const Timetable& timetable);
};

#endif