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

    double strengthMult;
    int strRounds;
    double defenseMult;
    int defRounds;

public:
    Player(std::string name);
    ~Player();

    void displayStats();
    Inventory& getInventory();

    int getGold() const;
    void addGold(int amount);
    bool spendGold(int amount);

    int getHealth() const;
    void takeDamage(int amount);
    void heal(int amount);

    Item* getEquippedWeapon();
    Item* getEquippedArmor();
    void equipItem(Item* item);

    double getStrengthMult() const;
    double getDefenseMult() const;
    bool applyPotion(bool isStr, double mult);
    void decrementBuffs();
};

#endif
