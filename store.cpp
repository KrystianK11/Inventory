#include "store.h"
#include <iostream>

Store::Store()
{
    stock.push_back(new Item("Axe", 150));
    stock.push_back(new Item("Bow", 200));
    stock.push_back(new Item("Helmet", 100));
    stock.push_back(new Item("Potion", 50));
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
    std::cout << "\n=== STORE ===\n";
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
