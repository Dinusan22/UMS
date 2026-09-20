#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <algorithm>

// Member 3: generic repository template.
template <typename T>
class Repository
{
private:
    std::vector<T*> items;

public:
    void add(T* item)
    {
        if (item != nullptr)
            items.push_back(item);
    }

    bool remove(T* item)
    {
        auto it = std::find(items.begin(), items.end(), item);
        if (it == items.end())
            return false;
        items.erase(it);
        return true;
    }

    T* findById(const std::string& id) const
    {
        for (T* item : items)
        {
            if (item != nullptr && item->getId() == id)
                return item;
        }
        return nullptr;
    }

    const std::vector<T*>& getAll() const { return items; }
    std::vector<T*>& getAll() { return items; }
    std::size_t size() const { return items.size(); }
};

#endif
