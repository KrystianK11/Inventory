#include "inventory.h"
#include <iostream>
#include <vector>
#include <algorithm>

Inventory::Inventory()
{
    rows = 4;
    cols = 4;
    currentRow = 0;
    currentCol = 0;
    isDragging = false;
    dragRow = -1;
    dragCol = -1;

    items = new Item**[rows];
    for(int i = 0; i < rows; i++) {
        items[i] = new Item*[cols];
        for(int j = 0; j < cols; j++) {
            items[i][j] = nullptr;
        }
    }
}

Inventory::~Inventory() {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if (items[i][j] != nullptr) {
                delete items[i][j];
            }
        }
        delete[] items[i];
    }
    delete[] items;
}

void Inventory::handleSwap()
{
    if (!isDragging) {
        isDragging = true;
        dragRow = currentRow;
        dragCol = currentCol;
    } else {
        if (dragRow == currentRow && dragCol == currentCol) {
            isDragging = false;
            dragRow = -1;
            dragCol = -1;
            return;
        }

        Item* temp = items[dragRow][dragCol];
        items[dragRow][dragCol] = items[currentRow][currentCol];
        items[currentRow][currentCol] = temp;

        isDragging = false;
        dragRow = -1;
        dragCol = -1;
    }
}

void Inventory::display(std::string filter, Item* equippedWeapon, Item* equippedArmor)
{
    std::cout << "\n=== INVENTORY ===\n";
    if(isDragging) {
        std::cout << "MOVE MODE\n";
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            bool isSelected = (i == currentRow && j == currentCol);
            bool isBeingDragged = (isDragging && i == dragRow && j == dragCol);

            if (isBeingDragged && isSelected) std::cout << ">}";
            else if (isBeingDragged)          std::cout << " {";
            else if (isSelected)              std::cout << ">[";
            else                              std::cout << " [";

            if (items[i][j] != nullptr) {
                if(filter == "ALL" || items[i][j]->getTypeString() == filter) {
                    bool isEq = (items[i][j] == equippedWeapon || items[i][j] == equippedArmor);
                    std::cout << items[i][j]->getName().at(0) << (isEq ? '*' : ' ');
                } else {
                    std::cout << ". ";
                }
            } else {
                std::cout << "  ";
            }

            if (isBeingDragged && isSelected) std::cout << "{<";
            else if (isBeingDragged)          std::cout << "} ";
            else if (isSelected)              std::cout << "]<";
            else                              std::cout << "] ";
        }
        std::cout << std::endl;
    }
}

void Inventory::displaySelectedInfo()
{
    std::cout << "\n=== DETAILS ===\n";
    if (items[currentRow][currentCol] != nullptr) {
        std::cout << "Name:       " << items[currentRow][currentCol]->getName() << "\n";
        std::cout << "Type:       " << items[currentRow][currentCol]->getTypeString() << "\n";
        std::cout << "Rarity:     " << items[currentRow][currentCol]->getRarityString() << "\n";
        std::cout << "Level:      " << items[currentRow][currentCol]->getLevel() << "\n";
        std::cout << "Stat Value: " << items[currentRow][currentCol]->getStat() << "\n";
        std::cout << "Durability: " << items[currentRow][currentCol]->getDurability() << "/100\n";
        std::cout << "Price:      " << items[currentRow][currentCol]->getPrice() << "G\n";
        std::cout << "Upgr. Cost: " << items[currentRow][currentCol]->getUpgradeCost() << "G\n";
    } else {
        std::cout << "[Empty Slot]\n";
    }
}

bool Inventory::addItem(Item* item)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (items[i][j] == nullptr) {
                items[i][j] = item;
                return true;
            }
        }
    }
    return false;
}

void Inventory::rearrangeInventory(std::string method) {
    std::vector<Item*> allItems;
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            if(items[i][j]) {
                allItems.push_back(items[i][j]);
                items[i][j] = nullptr;
            }
        }
    }

    if(method == "BEST") {
        std::sort(allItems.begin(), allItems.end(), [](Item* a, Item* b) {
            if (a->getLevel() != b->getLevel()) return a->getLevel() > b->getLevel();
            return a->getPrice() > b->getPrice();
        });
    } else if (method == "WORST") {
        std::sort(allItems.begin(), allItems.end(), [](Item* a, Item* b) {
            if (a->getLevel() != b->getLevel()) return a->getLevel() < b->getLevel();
            return a->getPrice() < b->getPrice();
        });
    } else if (method == "ABC") {
        std::sort(allItems.begin(), allItems.end(), [](Item* a, Item* b) {
            return a->getName() < b->getName();
        });
    }

    int index = 0;
    for(int i=0; i<rows && index < (int)allItems.size(); i++) {
        for(int j=0; j<cols && index < (int)allItems.size(); j++) {
            items[i][j] = allItems[index++];
        }
    }
}

void Inventory::deleteItemAtCurrent() {
    if(items[currentRow][currentCol] != nullptr) {
        delete items[currentRow][currentCol];
        items[currentRow][currentCol] = nullptr;
    }
}

int Inventory::getRows() const { return rows; }
void Inventory::setRows(int newRows) { rows = newRows; }
int Inventory::getCols() const { return cols; }
void Inventory::setCols(int newCols) { cols = newCols; }
Item ***Inventory::getItems() const { return items; }
void Inventory::setItems(Item ***newItems) { items = newItems; }
int Inventory::getCurrentRow() const { return currentRow; }
void Inventory::setCurrentRow(int newCurrentRow) { currentRow = newCurrentRow; }
int Inventory::getCurrentCol() const { return currentCol; }
void Inventory::setCurrentCol(int newCurrentCol) { currentCol = newCurrentCol; }
bool Inventory::getIsDragging() const { return isDragging; }
Item* Inventory::getItemAt(int r, int c) { return items[r][c]; }
Item* Inventory::takeItemAt(int r, int c) {
    Item* temp = items[r][c];
    items[r][c] = nullptr;
    return temp;
}
