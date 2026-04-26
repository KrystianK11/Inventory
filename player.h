#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "inventory.h"
#include "item.h"

class Player {
private:
    std::string name;
    int gold;
    int health;
    int maxHealth;
    int exp;
    int level;
    int fame;
    int mapX;
    int mapY;
    Inventory inventory;
    Item* equippedWeapon;
    Item* equippedArmor;
    double strengthMult;
    int strRounds;
    double defenseMult;
    int defRounds;
    double perkDmgBonus;
    double perkDefBonus;

public:
    Player(std::string name);
    ~Player();

    void displayStats();
    Inventory& getInventory();

    int getGold() const; void addGold(int amount); bool spendGold(int amount); void setGold(int amount);
    int getHealth() const; int getMaxHealth() const; void takeDamage(int amount); void heal(int amount); void setHealth(int amount); void setMaxHealth(int amount);
    int getExp() const; int getLevel() const; void addExp(int amount); void setLevelData(int l, int e);
    int getFame() const; void addFame(int amount); void setFame(int amount);
    int getX() const; int getY() const; void setPos(int x, int y);

    void addPerk(int choice);
    double getPerkDmg() const; double getPerkDef() const; void setPerks(double d, double df);

    Item* getEquippedWeapon(); Item* getEquippedArmor();
    void equipItem(Item* item); void unequipIfDeleted(Item* item); void clearEquipment();

    double getStrengthMult() const; double getDefenseMult() const;
    int getStrRounds() const; int getDefRounds() const;
    bool applyPotion(bool isStr, double mult); void decrementBuffs();
    void loadBuffs(double sM, int sR, double dM, int dR);
};
#endif
