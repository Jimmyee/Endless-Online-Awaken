// Endless Online Awaken v0.0.1

#ifndef INVENTORY_HPP_INCLUDED
#define INVENTORY_HPP_INCLUDED

#include <vector>

class Inventory
{
private:
    std::vector<std::pair<short, int>> items;

public:
    void AddItem(short id, int amount);
    void DelItem(short id, int amount);
    bool FindItem(short id, int amount);
    void Clear();
};

#endif // INVENTORY_HPP_INCLUDED
