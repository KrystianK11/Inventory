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

class PotionShop
{
private:
    std::vector<Item*> stock;
    int shopLevel;
    int upgradeCost;

public:
    PotionShop();
    ~PotionShop();
    void refreshStock();
    void displayStock();
    bool buyItem(Player& player, int itemIndex);
    void upgradeShop(Player& player);
};

#endif
