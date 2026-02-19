#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <cstdlib>

enum Rarity {
    common,
    magic,
    rare
};

class Item
{
protected:
    std::string name;
    Rarity rarity;
    int durability;
    int level;
    int price;
    int upgradeCost;
    double statMultiplier;
public:
    virtual ~Item() {}
    Item();
    Item(const std::string &name, int price);

    std::string getName() const;
    void setName(const std::string &newName);
    Rarity getRarity() const;
    void setRarity(Rarity newRarity);
    int getDurability() const;
    void setDurability(int newDurability);
    int getLevel() const;
    void setLevel(int newLevel);
    int getPrice() const;
    void setPrice(int newPrice);
    int getUpgradeCost() const;

    std::string getRarityString() const;
    virtual std::string getTypeString() const;
    virtual int getStat() const;

    virtual void use();
    void upgrade();
};

class Weapon : public Item {
private:
    int damage;
public:
    Weapon(std::string name, int price, int damage);
    int getStat() const override;
    std::string getTypeString() const override;
};

class Armor : public Item {
private:
    int defense;
public:
    Armor(std::string name, int price, int defense);
    int getStat() const override;
    std::string getTypeString() const override;
};

class Potion : public Item {
private:
    bool strengthType;
    int potLevel;
    double buffMultiplier;
public:
    Potion(bool isStrength, int level);
    std::string getTypeString() const override;
    bool isStrengthPotion() const;
    double getMultiplier() const;
};

#endif
