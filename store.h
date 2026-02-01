#ifndef STORE_H
#define STORE_H

#include <vector>
#include "item.h"
#include "player.h"

class Store
{
private:
    std::vector<Item*> stock;

public:
    Store();
    ~Store();
    void displayStock();
    bool buyItem(Player& player, int itemIndex);
};

#endif
