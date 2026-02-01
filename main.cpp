#include <iostream>
#include <cstdlib>
#include <ctime>
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

int main() {
    srand(time(NULL));

    Player hero("Hero");
    Store shop;

    hero.getInventory().addItem(new Item("Sword", 100));

    bool running = true;
    while (running) {
        system(CLEAR);

        std::cout << "OS: " << OS_NAME << "\n";

        hero.displayStats();
        hero.getInventory().display();

        std::cout << "\n[WASD] Move | [M] Swap | [I] Info | [B] Shop | [Q] Quit\n";

        char input = getSingleChar();

        Inventory& inv = hero.getInventory();
        int r = inv.getCurrentRow();
        int c = inv.getCurrentCol();

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
        case 'b':
            runStoreLoop(hero, shop);
            break;
        case 'i':
            if(!inv.getIsDragging()) {
                system(CLEAR);
                inv.displaySelectedInfo();
                getSingleChar();
            }
            break;
        case 'q':
            running = false;
            break;
        }
    }
    return 0;
}
