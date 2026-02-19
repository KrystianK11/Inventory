#include "item.h"

std::string Item::getName() const { return name; }
void Item::setName(const std::string &newName) { name = newName; }
Rarity Item::getRarity() const { return rarity; }
void Item::setRarity(Rarity newRarity) { rarity = newRarity; }
int Item::getDurability() const { return durability; }
void Item::setDurability(int newDurability) { durability = newDurability; }
int Item::getLevel() const { return level; }
void Item::setLevel(int newLevel) { level = newLevel; }
int Item::getPrice() const { return price; }
void Item::setPrice(int newPrice) { price = newPrice; }
int Item::getUpgradeCost() const { return upgradeCost; }

std::string Item::getRarityString() const {
    if(rarity == common) return "Common";
    if(rarity == magic) return "Magic";
    return "Rare";
}

std::string Item::getTypeString() const { return "Misc"; }
int Item::getStat() const { return 0; }

void Item::use() {
    if (durability > 0) {
        durability -= 5;
    }
}

void Item::upgrade() {
    level++;
    upgradeCost = (int)(upgradeCost * 1.5);
    statMultiplier *= 1.2;
    durability = 100;
}

Item::Item() {
    name = "Stick";
    price = 1;
    durability = 100;
    level = 1;
    rarity = magic;
    upgradeCost = 50;
    statMultiplier = 1.0;
}

Item::Item(const std::string &name, int price)
    : name(name), durability(100), level(1), price(price)
{
    upgradeCost = 50;
    statMultiplier = 1.0;
    int randNum = rand() % 101;
    if(randNum < 60) rarity = common;
    else if(randNum < 90) rarity = magic;
    else rarity = rare;
}

Weapon::Weapon(std::string name, int price, int damage) : Item(name, price), damage(damage) {}
int Weapon::getStat() const { return (int)(damage * statMultiplier); }
std::string Weapon::getTypeString() const { return "Weapon"; }

Armor::Armor(std::string name, int price, int defense) : Item(name, price), defense(defense) {}
int Armor::getStat() const { return (int)(defense * statMultiplier); }
std::string Armor::getTypeString() const { return "Armor"; }

Potion::Potion(bool isStrength, int lvl) : Item("", 0), strengthType(isStrength), potLevel(lvl) {
    if(strengthType) name = "Str Potion T" + std::to_string(lvl);
    else name = "Def Potion T" + std::to_string(lvl);

    if(lvl == 1) { buffMultiplier = 1.3; price = 50; }
    else if(lvl == 2) { buffMultiplier = 1.5; price = 100; }
    else if(lvl == 3) { buffMultiplier = 1.8; price = 200; }
    else if(lvl == 4) { buffMultiplier = 2.5; price = 400; }
    else if(lvl == 5) { buffMultiplier = 4.0; price = 800; }
}
std::string Potion::getTypeString() const { return "Potion"; }
bool Potion::isStrengthPotion() const { return strengthType; }
double Potion::getMultiplier() const { return buffMultiplier; }
