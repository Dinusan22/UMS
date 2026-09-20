#include "../include/Timetable.h"
#include <ostream>

// Member 3 integration definition for the operator declared by Member 2.
// The original Member 2 source files are not modified.
std::ostream& operator<<(std::ostream& out, const Timetable& timetable)
{
    out << "Timetable slots: " << timetable.getSlots().size() << '\n';
    for (size_t i = 0; i < timetable.getSlots().size(); ++i)
        out << "  Slot " << (i + 1) << " configured\n";
    return out;
}
