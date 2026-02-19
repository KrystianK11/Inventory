#include "store.h"
#include <iostream>

Store::Store()
{
    stock.push_back(new Weapon("Axe", 150, 15));
    stock.push_back(new Weapon("Bow", 200, 18));
    stock.push_back(new Armor("Helmet", 100, 10));
    stock.push_back(new Armor("Chestplate", 250, 25));
}

Store::~Store()
{
    for (size_t i = 0; i < stock.size(); ++i) {
        delete stock[i];
    }
    stock.clear();
}

void Store::displayStock()
{
    std::cout << "\n=== WEAPON & ARMOR STORE ===\n";
    for (size_t i = 0; i < stock.size(); ++i) {
        std::cout << "[" << i + 1 << "] "
                  << stock[i]->getName() << " ("
                  << stock[i]->getPrice() << "G)\n";
    }
    std::cout << "\nPress Number to buy, 'B' to exit.\n";
}

bool Store::buyItem(Player& player, int itemIndex)
{
    int actualIndex = itemIndex - 1;
    if (actualIndex < 0 || actualIndex >= (int)stock.size()) {
        return false;
    }

    Item* itemToBuy = stock[actualIndex];

    if (player.getGold() >= itemToBuy->getPrice()) {
        if (player.getInventory().addItem(itemToBuy)) {
            player.spendGold(itemToBuy->getPrice());
            stock.erase(stock.begin() + actualIndex);
            return true;
        }
    }
    return false;
}

PotionShop::PotionShop()
{
    shopLevel = 1;
    upgradeCost = 200;
    refreshStock();
}

PotionShop::~PotionShop()
{
    for (size_t i = 0; i < stock.size(); ++i) {
        delete stock[i];
    }
    stock.clear();
}

void PotionShop::refreshStock()
{
    for (size_t i = 0; i < stock.size(); ++i) {
        delete stock[i];
    }
    stock.clear();

    int amount = shopLevel + 1;
    for (int i = 0; i < amount; ++i) {
        bool isStr = (rand() % 2 == 0);
        int roll = rand() % 100;
        int lvl = 1;

        if (roll < 50) lvl = 1;
        else if (roll < 85) lvl = 2;
        else if (roll < 95) lvl = 3;
        else if (roll < 99) lvl = 4;
        else lvl = 5;

        stock.push_back(new Potion(isStr, lvl));
    }
}

void PotionShop::displayStock()
{
    std::cout << "\n=== POTION SHOP (Level " << shopLevel << ") ===\n";
    std::cout << "Chances: T1:50% | T2:35% | T3:10% | T4:4% | T5:1%\n";
    for (size_t i = 0; i < stock.size(); ++i) {
        std::cout << "[" << i + 1 << "] "
                  << stock[i]->getName() << " ("
                  << stock[i]->getPrice() << "G)\n";
    }
    std::cout << "\n[U] Upgrade Shop (" << upgradeCost << "G)\n";
    std::cout << "Press Number to buy, 'B' to exit.\n";
}

bool PotionShop::buyItem(Player& player, int itemIndex)
{
    int actualIndex = itemIndex - 1;
    if (actualIndex < 0 || actualIndex >= (int)stock.size()) {
        return false;
    }

    Item* itemToBuy = stock[actualIndex];

    if (player.getGold() >= itemToBuy->getPrice()) {
        if (player.getInventory().addItem(itemToBuy)) {
            player.spendGold(itemToBuy->getPrice());
            stock.erase(stock.begin() + actualIndex);
            return true;
        }
    }
    return false;
}

void PotionShop::upgradeShop(Player& player)
{
    if (player.getGold() >= upgradeCost) {
        player.spendGold(upgradeCost);
        shopLevel++;
        upgradeCost = (int)(upgradeCost * 2.0);
    }
}
