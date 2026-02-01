#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "inventory.h"
#include "item.h"

class Player
{
private:
    std::string name;
    int gold;
    int health;
    Inventory inventory;

    Item* equippedWeapon;
    Item* equippedArmor;

public:
    Player(std::string name);
    ~Player();

    void displayStats();
    Inventory& getInventory();

    int getGold() const;
    void addGold(int amount);
    bool spendGold(int amount);
};

#endif
