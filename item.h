#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <time.h>

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
    void use();
};

#endif
