#include "player.h"
#include <iostream>

Player::Player(std::string name)
{
    this->name = name;
    this->gold = 500;
    this->health = 100;
    this->equippedWeapon = nullptr;
    this->equippedArmor = nullptr;
}

Player::~Player()
{
}

Inventory& Player::getInventory()
{
    return inventory;
}

int Player::getGold() const
{
    return gold;
}

void Player::addGold(int amount)
{
    gold += amount;
}

bool Player::spendGold(int amount)
{
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

void Player::displayStats()
{
    std::cout << "Name: " << name << " | Gold: " << gold << " | HP: " << health << std::endl;
}
