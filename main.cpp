#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include "player.h"
#include "item.h"
#include "store.h"

#ifdef _WIN32
#include <conio.h>
#define CLEAR "cls"
#else
#include <termios.h>
#include <unistd.h>
#define CLEAR "clear"
#endif

char getSingleChar() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt; char ch; tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt; newt.c_lflag &= ~(ICANON | ECHO); tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar(); tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return ch;
#endif
}

const int MAP_SIZE = 15;
int totalKills = 0;
int questTarget = 3;

void loadMeta(Player& hero) {
    std::ifstream file("meta.dat");
    if (file.is_open()) {
        int f; if(file >> f) hero.setFame(f);
        file.close();
    }
    hero.addGold(hero.getFame() * 50);
}

void saveMeta(Player& hero) {
    std::ofstream file("meta.dat");
    if (file.is_open()) { file << hero.getFame(); file.close(); }
}

void saveGame(Player& hero) {
    std::ofstream file("savegame.dat");
    if (file.is_open()) {
        file << hero.getGold() << " " << hero.getHealth() << " " << hero.getMaxHealth() << " "
             << hero.getExp() << " " << hero.getLevel() << " " << hero.getX() << " " << hero.getY() << " " << totalKills << " " << questTarget << "\n";
        file << hero.getStrengthMult() << " " << hero.getStrRounds() << " " << hero.getDefenseMult() << " " << hero.getDefRounds() << "\n";
        file << hero.getPerkDmg() << " " << hero.getPerkDef() << "\n";

        Inventory& inv = hero.getInventory();
        file << inv.getRows() << " " << inv.getCols() << "\n";

        for(int i = 0; i < inv.getRows(); i++) {
            for(int j = 0; j < inv.getCols(); j++) {
                Item* it = inv.getItemAt(i, j);
                if(it) {
                    int eF = 0; if(it==hero.getEquippedWeapon()) eF=1; else if(it==hero.getEquippedArmor()) eF=2;
                    file << i << " " << j << " " << it->getTypeString() << " " << it->getName() << " " << it->getPrice() << " " << it->getLevel() << " " << it->getDurability() << " ";
                    if (it->getTypeString() == "Potion") {
                        Potion* p = static_cast<Potion*>(it); file << p->getPotLevel() << " " << p->isStrengthPotion() << " " << eF << "\n";
                    } else { file << it->getBaseStat() << " 0 " << eF << "\n"; }
                }
            }
        }
        file.close();
    }
    saveMeta(hero);
}

bool loadGame(Player& hero) {
    std::ifstream file("savegame.dat");
    if (file.is_open()) {
        int g, h, mh, e, l, x, y, tk, qt;
        if (file >> g >> h >> mh >> e >> l >> x >> y >> tk >> qt) {
            hero.setGold(g); hero.setHealth(h); hero.setMaxHealth(mh); hero.setLevelData(l, e); hero.setPos(x, y); totalKills = tk; questTarget = qt;
            double sm, dm, pd, pdf; int sr, dr;
            if (file >> sm >> sr >> dm >> dr) hero.loadBuffs(sm, sr, dm, dr);
            if (file >> pd >> pdf) hero.setPerks(pd, pdf);

            Inventory& inv = hero.getInventory(); hero.clearEquipment();
            for(int i=0; i<inv.getRows(); i++) for(int j=0; j<inv.getCols(); j++) { if(inv.getItemAt(i,j)){ delete inv.getItemAt(i,j); inv.getItems()[i][j]=nullptr; } }

            int rows, cols;
            if (file >> rows >> cols) {
                int ri, ci, pr, lvl, dur, bs, eF; std::string ty, na; bool isS;
                while (file >> ri >> ci >> ty >> na >> pr >> lvl >> dur >> bs >> isS >> eF) {
                    Item* nI = nullptr;
                    if (ty == "Weapon") nI = new Weapon(na, pr, bs); else if (ty == "Armor") nI = new Armor(na, pr, bs); else if (ty == "Potion") nI = new Potion(isS, bs);
                    if (nI) {
                        for(int k=1; k<lvl; k++) nI->upgrade();
                        nI->setDurability(dur); inv.getItems()[ri][ci] = nI;
                        if(eF==1) hero.equipItem(nI); else if(eF==2) hero.equipItem(nI);
                    }
                }
            }
            file.close(); return true;
        }
    }
    return false;
}

void levelUpCheck(Player& hero) {
    int required = hero.getLevel() * 50;
    if (hero.getExp() >= required) {
        system(CLEAR);
        hero.setLevelData(hero.getLevel() + 1, hero.getExp() - required);
        hero.setMaxHealth(hero.getMaxHealth() + 20);
        hero.heal(999);
        std::cout << "LEVEL UP! You are now level " << hero.getLevel() << "!\n";
        std::cout << "Choose a Perk:\n[1] +20% Base Damage\n[2] +20% Base Defense\n";
        char p = getSingleChar();
        if (p == '1') hero.addPerk(1); else hero.addPerk(2);
    }
}

void runAnvil(Player& hero) {
    system(CLEAR);
    Inventory& inv = hero.getInventory();
    std::cout << "=== ANVIL (Combine 2 identical level items) ===\n";
    inv.display("ALL", hero.getEquippedWeapon(), hero.getEquippedArmor());

    int r1, c1, r2, c2;
    std::cout << "Enter coords for Item 1 (Row Col) or type '-1' to exit: ";
    if (!(std::cin >> r1) || r1 == -1) {
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }
    if (!(std::cin >> c1)) {
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }

    std::cout << "Enter coords for Item 2 (Row Col) or type '-1' to exit: ";
    if (!(std::cin >> r2) || r2 == -1) {
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }
    if (!(std::cin >> c2)) {
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }

    // Zabezpieczenie przed wpisaniem zbyt dużych liczb (Crash zapobiegnięty)
    if (r1 < 0 || r1 >= inv.getRows() || c1 < 0 || c1 >= inv.getCols() ||
        r2 < 0 || r2 >= inv.getRows() || c2 < 0 || c2 >= inv.getCols()) {
        std::cout << "\nFailed. Invalid coordinates!\n";
        std::cin.clear(); std::cin.ignore(10000, '\n');
        std::cout << "Press any key to continue..."; getSingleChar();
        return;
    }

    Item* i1 = inv.getItemAt(r1, c1);
    Item* i2 = inv.getItemAt(r2, c2);

    if(i1 && i2 && i1 != i2 && i1->getName() == i2->getName() && i1->getLevel() == i2->getLevel() && i1->getTypeString() != "Potion") {
        hero.unequipIfDeleted(i1); hero.unequipIfDeleted(i2);
        inv.takeItemAt(r1, c1); inv.takeItemAt(r2, c2);
        i1->upgrade(); i1->setPrice(i1->getPrice()*2);
        inv.addItem(i1); delete i2;
        std::cout << "\nSuccess! Items combined.\n";
    } else {
        std::cout << "\nFailed. Items must be identical type and level.\n";
    }

    std::cin.clear(); std::cin.ignore(10000, '\n');
    std::cout << "Press any key to continue...";
    getSingleChar();
}

int runCombat(Player& hero, bool isBoss) {
    int eHP = (20 * hero.getLevel()) + (isBoss ? 100 : 0);
    int eMax = eHP;
    int eAtk = (8 * hero.getLevel()) + (isBoss ? 15 : 0);
    int ticks = 0; std::string log = isBoss ? "BOSS ENCOUNTER!" : "Enemy appeared!";

    int pDmg = 5; if(hero.getEquippedWeapon()) pDmg = hero.getEquippedWeapon()->getStat();
    pDmg = (int)(pDmg * hero.getStrengthMult() * hero.getPerkDmg());
    int pDef = 0; if(hero.getEquippedArmor()) pDef = hero.getEquippedArmor()->getStat();
    pDef = (int)(pDef * hero.getDefenseMult() * hero.getPerkDef());

    while(eHP > 0 && hero.getHealth() > 0) {
        system(CLEAR);
        std::cout << (isBoss ? "=== BOSS BATTLE ===\n" : "=== BATTLE ===\n");
        std::cout << "HP: " << hero.getHealth() << " | DEF: " << pDef << " | ATK: " << pDmg << "\n";
        std::cout << "Enemy HP: " << eHP << "/" << eMax << " | ATK: " << eAtk << "\n";
        std::cout << "Enemy attacks in: " << (3 - ticks) << "\n-------------------\n" << log << "\n-------------------\n";
        std::cout << "[A]ttack | [S]pecial | [B]lock | [R]un\n";

        char in = tolower(getSingleChar()); log = ""; bool blk = false;
        if(in == 'a') { eHP -= pDmg; log = "Attacked for " + std::to_string(pDmg); if(hero.getEquippedWeapon()) hero.getEquippedWeapon()->use(); }
        else if(in == 's') { if(hero.getHealth()>20) { hero.takeDamage(20); eHP -= (pDmg*3); log = "SPECIAL! Dealt " + std::to_string(pDmg*3); } else {continue;} }
        else if(in == 'b') { blk = true; log = "Blocking!"; if(hero.getEquippedArmor()) hero.getEquippedArmor()->use();}
        else if(in == 'r') { return 0; } else { continue; }

        if(eHP <= 0) break;
        ticks++;
        if(ticks >= 3) {
            int dmg = isBoss ? eAtk * 1.5 : eAtk; if(blk) dmg -= pDef; if(dmg<0) dmg=0;
            hero.takeDamage(dmg); log += " | Enemy hit for " + std::to_string(dmg); ticks = 0;
        }
    }
    system(CLEAR);
    if(hero.getHealth() > 0) {
        int rG = (30 * hero.getLevel()) + (isBoss ? 200 : 0);
        int rE = (25 * hero.getLevel()) + (isBoss ? 100 : 0);
        std::cout << "VICTORY! Gained " << rG << " GOLD and " << rE << " EXP.\n";
        hero.addGold(rG); hero.addExp(rE); totalKills++; levelUpCheck(hero);
        getSingleChar(); return 1;
    }
    return -1;
}

int main() {
    srand(time(NULL));
    bool play = true;

    while (play) {
        Player hero("Hero"); Store shop; PotionShop pShop;
        std::string filter = "ALL";
        bool inInventory = false;

        hero.getInventory().addItem(new Weapon("Rusty_Sword", 10, 8));
        loadMeta(hero);

        if(!loadGame(hero)) { totalKills = 0; questTarget = 3; }

        bool run = true;
        while (run) {
            system(CLEAR);
            int px = hero.getX(); int py = hero.getY();
            Inventory& inv = hero.getInventory();

            hero.displayStats();

            if (inInventory) {
                inv.display(filter, hero.getEquippedWeapon(), hero.getEquippedArmor());

                // --- INFO O PRZEDMIOCIE NA KTÓRYM STOIMY (HOVER) ---
                int r = inv.getCurrentRow();
                int c = inv.getCurrentCol();
                Item* sel = inv.getItemAt(r, c);

                std::cout << "----------------------------------------------\n";
                if (sel) {
                    std::cout << "[Hover] " << sel->getName() << " | Lv." << sel->getLevel()
                    << " " << sel->getRarityString() << " " << sel->getTypeString()
                    << " | Stat: " << sel->getStat() << " | Value: " << sel->getPrice() << "G\n";
                } else {
                    std::cout << "[Hover] --- Empty Slot ---\n";
                }
                std::cout << "----------------------------------------------\n";
                // ---------------------------------------------------

                std::cout << "[WASD] Move Cursor | [M] Swap | [E] Equip/Use | [I] Close Inventory\n";
                std::cout << "[X] Drop | [$] Sell | [Z] Sort/Filter Menu\n";

                char in = tolower(getSingleChar());

                switch (in) {
                case 'w': if(r>0) inv.setCurrentRow(r-1); break;
                case 's': if(r<inv.getRows()-1) inv.setCurrentRow(r+1); break;
                case 'a': if(c>0) inv.setCurrentCol(c-1); break;
                case 'd': if(c<inv.getCols()-1) inv.setCurrentCol(c+1); break;
                case 'i': inInventory = false; break;
                case 'm': inv.handleSwap(); break;
                case 'x': if(sel){ hero.unequipIfDeleted(sel); inv.deleteItemAtCurrent(); } break;

                // --- NAPRAWIONE SPRZEDAWANIE ---
                case '$':
                    if(sel) {
                        int sellValue = sel->getPrice() / 2;
                        if (sellValue < 1) sellValue = 1;

                        hero.addGold(sellValue);
                        hero.unequipIfDeleted(sel);
                        inv.deleteItemAtCurrent();

                        std::cout << "\n[!] Sold item for " << sellValue << " GOLD! Press any key...";
                        getSingleChar();
                    }
                    break;

                case 'z':
                    system(CLEAR); std::cout << "[1] Best [2] Worst [3] ABC [4] Compact\n"; char sIn; sIn=getSingleChar();
                    if(sIn=='1')inv.rearrangeInventory("BEST"); else if(sIn=='2')inv.rearrangeInventory("WORST"); else if(sIn=='3')inv.rearrangeInventory("ABC"); else if(sIn=='4')inv.rearrangeInventory("COMPACT");
                    break;
                case 'e':
                    if (sel) {
                        if (sel->getTypeString() == "Potion") {
                            Potion* p = static_cast<Potion*>(sel);
                            if (hero.applyPotion(p->isStrengthPotion(), p->getMultiplier())) inv.deleteItemAtCurrent();
                        } else hero.equipItem(sel);
                    } break;
                }
            } else {
                std::cout << "\n=== WORLD MAP ===\n";
                std::cout << "  +------------------------------+\n";
                for(int y=0; y<MAP_SIZE; y++) {
                    std::cout << "  |";
                    for(int x=0; x<MAP_SIZE; x++) {
                        if(x==px && y==py) std::cout << "@ ";
                        else if(x==0 && y==0) std::cout << "T ";
                        else {
                            int noise = (x * 17 + y * 31) % 10;
                            if(noise == 0) std::cout << "^ ";
                            else if (noise == 1) std::cout << "\" ";
                            else std::cout << ". ";
                        }
                    }
                    std::cout << "|\n";
                }
                std::cout << "  +------------------------------+\n";
                if (totalKills >= questTarget) {
                    std::cout << "Quest: COMPLETE! Return to Town (T) to claim.\n";
                } else {
                    std::cout << "Quest: Kill " << totalKills << "/" << questTarget << " enemies.\n";
                }

                std::cout << "\n[WASD] Walk | [I] Open Inventory | [Q] Save & Quit\n";
                if(px==0 && py==0) std::cout << "TOWN: [B] Store | [P] Potions | [V] Anvil | [H] Heal(50G) | [C]laim Quest\n";

                char in = tolower(getSingleChar());
                switch (in) {
                case 'w': if(py>0) hero.setPos(px, py-1); break;
                case 's': if(py<MAP_SIZE-1) hero.setPos(px, py+1); break;
                case 'a': if(px>0) hero.setPos(px-1, py); break;
                case 'd': if(px<MAP_SIZE-1) hero.setPos(px+1, py); break;
                case 'i': inInventory = true; break;
                case 'q': saveGame(hero); run = false; play = false; break;
                case 'b': if(px==0 && py==0) { bool s=true; while(s){ system(CLEAR); std::cout<<"GOLD: "<<hero.getGold()<<"\n"; shop.displayStock(); char si=getSingleChar(); if(si=='b')s=false; else if(isdigit(si)) shop.buyItem(hero,si-'0'); } } break;
                case 'p': if(px==0 && py==0) { bool s=true; while(s){ system(CLEAR); std::cout<<"GOLD: "<<hero.getGold()<<"\n"; pShop.displayStock(); char si=getSingleChar(); if(si=='b')s=false; else if(si=='u') pShop.upgradeShop(hero); else if(isdigit(si)) pShop.buyItem(hero,si-'0'); } } break;
                case 'v': if(px==0 && py==0) runAnvil(hero); break;
                case 'h': if(px==0 && py==0 && hero.getGold()>=50) { hero.spendGold(50); hero.heal(999); } break;
                case 'c': if(px==0 && py==0 && totalKills >= questTarget) { hero.addGold(questTarget * 25); totalKills -= questTarget; questTarget += 2; std::cout<<"Quest Claimed!\nPress key..."; getSingleChar(); } break;
                }

                if(in == 'w' || in == 'a' || in == 's' || in == 'd') {
                    if((hero.getX() != 0 || hero.getY() != 0) && (rand() % 100 < 25)) {
                        int bRes = runCombat(hero, totalKills > 0 && totalKills % 5 == 0);
                        if (bRes == 1) { hero.decrementBuffs(); saveGame(hero); }
                        else if (bRes == -1) {
                            system(CLEAR); std::cout << "\n\n  Y O U   D I E D \n\n Fame Gained: " << hero.getLevel() << "\n\nPress key...\n";
                            hero.addFame(hero.getLevel()); saveMeta(hero); std::remove("savegame.dat");
                            getSingleChar(); run = false;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
