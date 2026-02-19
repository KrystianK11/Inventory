#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "player.h"
#include "item.h"
#include "store.h"

#ifdef _WIN32
#include <conio.h>
#define OS_NAME "Windows"
#define CLEAR "cls"
#elif __APPLE__
#include <TargetConditionals.h>
#include <termios.h>
#include <unistd.h>
#if TARGET_OS_MAC
#define OS_NAME "macOS"
#define CLEAR "clear"
#endif
#else
#define OS_NAME "Unknown OS"
#define CLEAR "clear"
#endif

char getSingleChar() {
#ifdef _WIN32
    return _getch();
#elif __APPLE__
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

void runStoreLoop(Player& hero, Store& shop) {
    bool shopping = true;
    while(shopping) {
        system(CLEAR);
        std::cout << "Gold: " << hero.getGold() << "\n";
        shop.displayStock();

        char input = getSingleChar();
        if (tolower(input) == 'b') {
            shopping = false;
        }
        else if (isdigit(input)) {
            int selection = input - '0';
            shop.buyItem(hero, selection);
        }
    }
}

void runPotionShopLoop(Player& hero, PotionShop& pShop) {
    bool shopping = true;
    while(shopping) {
        system(CLEAR);
        std::cout << "Gold: " << hero.getGold() << "\n";
        pShop.displayStock();

        char input = getSingleChar();
        if (tolower(input) == 'b') {
            shopping = false;
        }
        else if (tolower(input) == 'u') {
            pShop.upgradeShop(hero);
        }
        else if (isdigit(input)) {
            int selection = input - '0';
            pShop.buyItem(hero, selection);
        }
    }
}

int runMiniGame(Player& hero, int& currentRound) {
    int enemyMaxHP = 20 + (currentRound - 1) * 20 + (currentRound * currentRound * 2);
    int enemyHP = enemyMaxHP;
    int enemyAtk = 10 + (currentRound - 1) * 8 + (currentRound * currentRound);
    int enemyAttackRate = 3;
    int tickCounter = 0;

    int playerDmg = 5;
    if(hero.getEquippedWeapon()) {
        playerDmg = hero.getEquippedWeapon()->getStat();
    }
    playerDmg = (int)(playerDmg * hero.getStrengthMult());

    int playerDef = 0;
    if(hero.getEquippedArmor()) {
        playerDef = hero.getEquippedArmor()->getStat();
    }
    playerDef = (int)(playerDef * hero.getDefenseMult());

    std::string actionLog = "A wild enemy appears!";

    while (enemyHP > 0 && hero.getHealth() > 0) {
        system(CLEAR);
        std::cout << "=== BATTLE - ROUND " << currentRound << " ===\n";
        std::cout << "Player HP: " << hero.getHealth() << "/100  |  Armor: " << playerDef << "  |  Weapon Dmg: " << playerDmg << "\n";
        std::cout << "Enemy HP:  " << enemyHP << "/" << enemyMaxHP << "  |  Enemy Atk: " << enemyAtk << "\n";
        std::cout << "Enemy attacks in: " << (enemyAttackRate - tickCounter) << " action(s)\n";
        std::cout << "------------------------------------------------\n";
        std::cout << "LOG: " << actionLog << "\n";
        std::cout << "------------------------------------------------\n";
        std::cout << "[A]ttack | [B]lock | [R]un away\n";

        char input = getSingleChar();
        actionLog = "";
        bool isBlocking = false;

        if (tolower(input) == 'a') {
            enemyHP -= playerDmg;
            if(hero.getEquippedWeapon()) hero.getEquippedWeapon()->use();
            actionLog = "You attacked for " + std::to_string(playerDmg) + " damage! ";
        } else if (tolower(input) == 'b') {
            isBlocking = true;
            if(hero.getEquippedArmor()) hero.getEquippedArmor()->use();
            actionLog = "You brace yourself to block! ";
        } else if (tolower(input) == 'r') {
            std::cout << "You ran away safely.\nPress any key...";
            getSingleChar();
            return 0;
        } else {
            continue;
        }

        if (enemyHP <= 0) {
            break;
        }

        tickCounter++;
        if (tickCounter >= enemyAttackRate) {
            int incomingDmg = enemyAtk;
            if (isBlocking) {
                incomingDmg -= playerDef;
                if (incomingDmg < 0) incomingDmg = 0;
                actionLog += "Enemy attacks! You blocked, taking " + std::to_string(incomingDmg) + " damage.";
            } else {
                actionLog += "Enemy attacks and hits for " + std::to_string(incomingDmg) + " damage!";
            }
            hero.takeDamage(incomingDmg);
            tickCounter = 0;
        } else {
            actionLog += "Enemy is preparing to attack...";
        }
    }

    system(CLEAR);
    if (hero.getHealth() > 0 && enemyHP <= 0) {
        int reward = (rand() % 30 + 20) + (currentRound * 15) + (playerDmg * 2) + (playerDef * 2);
        std::cout << "=== VICTORY ===\n";
        std::cout << actionLog << "\n\n";
        std::cout << "You defeated the round " << currentRound << " enemy!\n";
        std::cout << "You earned " << reward << " Gold.\n";
        hero.addGold(reward);
        currentRound++;
        std::cout << "\nPress any key to return...";
        getSingleChar();
        return 1;
    } else if (hero.getHealth() <= 0) {
        return -1;
    }
    return 0;
}

int main() {
    srand(time(NULL));

    bool keepPlaying = true;

    while (keepPlaying) {
        Player hero("Hero");
        Store shop;
        PotionShop pShop;
        int currentRound = 1;

        hero.getInventory().addItem(new Weapon("Rusty Sword", 100, 8));

        bool running = true;
        while (running) {
            system(CLEAR);

            hero.displayStats();
            hero.getInventory().display(hero.getEquippedWeapon(), hero.getEquippedArmor());

            std::cout << "\n[WASD] Move | [M] Swap | [E] Equip/Use | [I] Info\n";
            std::cout << "[U] Upgrade | [F] Fight| [B] Store     | [P] Potion Shop\n";
            std::cout << "[H] Heal (50G) | [Q] Quit\n";

            char input = getSingleChar();

            Inventory& inv = hero.getInventory();
            int r = inv.getCurrentRow();
            int c = inv.getCurrentCol();
            Item* selectedItem = inv.getItems()[r][c];

            switch (tolower(input)) {
            case 'w':
                if (r > 0) inv.setCurrentRow(r - 1);
                break;
            case 's':
                if (r < inv.getRows() - 1) inv.setCurrentRow(r + 1);
                break;
            case 'a':
                if (c > 0) inv.setCurrentCol(c - 1);
                break;
            case 'd':
                if (c < inv.getCols() - 1) inv.setCurrentCol(c + 1);
                break;
            case 'm':
                inv.handleSwap();
                break;
            case 'e':
                if (selectedItem) {
                    if (selectedItem->getTypeString() == "Potion") {
                        Potion* p = static_cast<Potion*>(selectedItem);
                        if (hero.applyPotion(p->isStrengthPotion(), p->getMultiplier())) {
                            inv.getItems()[r][c] = nullptr;
                            delete selectedItem;
                        }
                    } else {
                        hero.equipItem(selectedItem);
                    }
                }
                break;
            case 'u':
                if (selectedItem && selectedItem->getTypeString() != "Potion") {
                    int cost = selectedItem->getUpgradeCost();
                    if (hero.getGold() >= cost) {
                        hero.spendGold(cost);
                        selectedItem->upgrade();
                    }
                }
                break;
            case 'h':
                if (hero.getGold() >= 50 && hero.getHealth() < 100) {
                    hero.spendGold(50);
                    hero.heal(100);
                }
                break;
            case 'f':
            {
                int battleResult = runMiniGame(hero, currentRound);
                if (battleResult == 1) {
                    hero.decrementBuffs();
                    if ((currentRound - 1) % 5 == 0 && (currentRound - 1) > 0) {
                        pShop.refreshStock();
                    }
                } else if (battleResult == -1) {
                    system(CLEAR);
                    std::cout << "\n\n\n";
                    std::cout << "     =================================\n";
                    std::cout << "               Y O U   D I E D        \n";
                    std::cout << "     =================================\n\n";
                    std::cout << "       You survived until Round " << currentRound << ".\n\n";
                    std::cout << "     Press any key to respawn...\n";
                    getSingleChar();
                    running = false;
                }
                break;
            }
            case 'b':
                runStoreLoop(hero, shop);
                break;
            case 'p':
                runPotionShopLoop(hero, pShop);
                break;
            case 'i':
                if(!inv.getIsDragging()) {
                    system(CLEAR);
                    inv.displaySelectedInfo();
                    std::cout << "\nPress any key to return...";
                    getSingleChar();
                }
                break;
            case 'q':
                running = false;
                keepPlaying = false;
                break;
            }
        }
    }
    return 0;
}
