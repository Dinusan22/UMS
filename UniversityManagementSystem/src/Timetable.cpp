#include "../include/Timetable.h"

void Timetable::addSlot(const TimeSlot& slot)
{
    slots.push_back(slot);
}

void Timetable::removeSlot(const TimeSlot& slot)
{
    for (auto it = slots.begin(); it != slots.end(); ++it)
    {
        if (*it == slot)
        {
            slots.erase(it);
            return;
        }
    }
}

bool Timetable::hasClash(const TimeSlot& slot) const
{
    for (const TimeSlot& existingSlot : slots)
    {
        if (existingSlot.overlaps(slot))
        {
            return true;
        }
    }

    return false;
}