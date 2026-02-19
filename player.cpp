#include "player.h"
#include <iostream>

Player::Player(std::string name)
{
    this->name = name;
    this->gold = 100;
    this->health = 100;
    this->equippedWeapon = nullptr;
    this->equippedArmor = nullptr;
    this->strengthMult = 1.0;
    this->strRounds = 0;
    this->defenseMult = 1.0;
    this->defRounds = 0;
}

Player::~Player() {}

Inventory& Player::getInventory() { return inventory; }
int Player::getGold() const { return gold; }
void Player::addGold(int amount) { gold += amount; }

bool Player::spendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

int Player::getHealth() const { return health; }
void Player::takeDamage(int amount) { health -= amount; }
void Player::heal(int amount) {
    health += amount;
    if(health > 100) health = 100;
}

Item* Player::getEquippedWeapon() { return equippedWeapon; }
Item* Player::getEquippedArmor() { return equippedArmor; }

void Player::equipItem(Item* item) {
    if(!item) return;
    if(item->getTypeString() == "Weapon") equippedWeapon = item;
    else if(item->getTypeString() == "Armor") equippedArmor = item;
}

double Player::getStrengthMult() const { return strengthMult; }
double Player::getDefenseMult() const { return defenseMult; }

bool Player::applyPotion(bool isStr, double mult) {
    if(isStr) {
        if(strRounds > 0 && strengthMult > mult) return false;
        strengthMult = mult;
        strRounds = 5;
        return true;
    } else {
        if(defRounds > 0 && defenseMult > mult) return false;
        defenseMult = mult;
        defRounds = 5;
        return true;
    }
}

void Player::decrementBuffs() {
    if(strRounds > 0) {
        strRounds--;
        if(strRounds == 0) strengthMult = 1.0;
    }
    if(defRounds > 0) {
        defRounds--;
        if(defRounds == 0) defenseMult = 1.0;
    }
}

void Player::displayStats()
{
    std::cout << "Name: " << name << " | HP: " << health << "/100 | Gold: " << gold << "\n";
    std::cout << "Weapon: " << (equippedWeapon ? equippedWeapon->getName() : "None") << " | ";
    std::cout << "Armor: " << (equippedArmor ? equippedArmor->getName() : "None") << "\n";
    std::cout << "Buffs: STR x" << strengthMult << " (" << strRounds << " rounds) | DEF x" << defenseMult << " (" << defRounds << " rounds)\n";
}
