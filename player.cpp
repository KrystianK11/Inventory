#include "player.h"
#include <iostream>

Player::Player(std::string name) : name(name), gold(0), health(100), maxHealth(100), exp(0), level(1), fame(0), mapX(0), mapY(0), equippedWeapon(nullptr), equippedArmor(nullptr), strengthMult(1.0), strRounds(0), defenseMult(1.0), defRounds(0), perkDmgBonus(1.0), perkDefBonus(1.0) {}
Player::~Player() {}

Inventory& Player::getInventory() { return inventory; }
int Player::getGold() const { return gold; }
void Player::addGold(int amount) { gold += amount; }
void Player::setGold(int amount) { gold = amount; }
bool Player::spendGold(int amount) { if(gold>=amount){gold-=amount; return true;} return false; }
int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
void Player::takeDamage(int amount) { health -= amount; }
void Player::heal(int amount) { health += amount; if(health > maxHealth) health = maxHealth; }
void Player::setHealth(int amount) { health = amount; }
void Player::setMaxHealth(int amount) { maxHealth = amount; }
int Player::getExp() const { return exp; }
int Player::getLevel() const { return level; }
void Player::addExp(int amount) { exp += amount; }
void Player::setLevelData(int l, int e) { level = l; exp = e; }
int Player::getFame() const { return fame; }
void Player::addFame(int amount) { fame += amount; }
void Player::setFame(int amount) { fame = amount; }
int Player::getX() const { return mapX; }
int Player::getY() const { return mapY; }
void Player::setPos(int x, int y) { mapX = x; mapY = y; }
void Player::addPerk(int choice) { if(choice==1) perkDmgBonus+=0.2; else perkDefBonus+=0.2; }
double Player::getPerkDmg() const { return perkDmgBonus; }
double Player::getPerkDef() const { return perkDefBonus; }
void Player::setPerks(double d, double df) { perkDmgBonus = d; perkDefBonus = df; }

Item* Player::getEquippedWeapon() { return equippedWeapon; }
Item* Player::getEquippedArmor() { return equippedArmor; }
void Player::equipItem(Item* item) {
    if(!item) return;
    if(item->getTypeString() == "Weapon") equippedWeapon = item;
    else if(item->getTypeString() == "Armor") equippedArmor = item;
}
void Player::unequipIfDeleted(Item* item) { if(equippedWeapon == item) equippedWeapon = nullptr; if(equippedArmor == item) equippedArmor = nullptr; }
void Player::clearEquipment() { equippedWeapon = nullptr; equippedArmor = nullptr; }

double Player::getStrengthMult() const { return strengthMult; }
double Player::getDefenseMult() const { return defenseMult; }
int Player::getStrRounds() const { return strRounds; }
int Player::getDefRounds() const { return defRounds; }
bool Player::applyPotion(bool isStr, double mult) {
    if(isStr) { if(strRounds>0 && strengthMult>mult) return false; strengthMult=mult; strRounds=5; return true; }
    else { if(defRounds>0 && defenseMult>mult) return false; defenseMult=mult; defRounds=5; return true; }
}
void Player::decrementBuffs() {
    if(strRounds>0) { strRounds--; if(strRounds==0) strengthMult=1.0; }
    if(defRounds>0) { defRounds--; if(defRounds==0) defenseMult=1.0; }
}
void Player::loadBuffs(double sM, int sR, double dM, int dR) { strengthMult=sM; strRounds=sR; defenseMult=dM; defRounds=dR; }

void Player::displayStats() {
    // --- OBLICZANIE PASKA EXP ---
    int requiredExp = level * 50;
    int barWidth = 20; // Szerokość paska (możesz zmienić na 30 lub 10)
    int filled = (int)(((float)exp / requiredExp) * barWidth);

    std::string expBar = "[";
    for(int i = 0; i < barWidth; ++i) {
        if(i < filled) expBar += "#";  // Wypełniona część
        else expBar += "-";            // Pusta część
    }
    expBar += "]";
    // ----------------------------

    // Wyświetlanie górnej belki (wyrzuciłem stąd EXP, żeby miało własną linijkę)
    std::cout << "Lv." << level << " " << name << " | HP: " << health << "/" << maxHealth << " | GOLD: " << gold << " | Fame: " << fame << "\n";

    // Nowa linijka dedykowana dla EXP i paska
    std::cout << "EXP: " << exp << "/" << requiredExp << " " << expBar << "\n";

    // Reszta statystyk
    std::cout << "Wep: " << (equippedWeapon ? equippedWeapon->getName() : "None") << " | Arm: " << (equippedArmor ? equippedArmor->getName() : "None") << "\n";
    std::cout << "Buffs: STR x" << strengthMult << " DEF x" << defenseMult << " | Perks: DMG+" << (perkDmgBonus-1)*100 << "% DEF+" << (perkDefBonus-1)*100 << "%\n";
}
