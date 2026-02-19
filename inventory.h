#ifndef INVENTORY_H
#define INVENTORY_H
#include "item.h"

class Inventory
{
private:
    int rows;
    int cols;
    Item*** items;
    int currentRow;
    int currentCol;

    bool isDragging;
    int dragRow;
    int dragCol;

public:
    Inventory();
    ~Inventory();

    void display(Item* equippedWeapon = nullptr, Item* equippedArmor = nullptr);
    void displaySelectedInfo();
    bool addItem(Item* item);
    void handleSwap();

    int getRows() const;
    void setRows(int newRows);
    int getCols() const;
    void setCols(int newCols);
    Item ***getItems() const;
    void setItems(Item ***newItems);
    int getCurrentRow() const;
    void setCurrentRow(int newCurrentRow);
    int getCurrentCol() const;
    void setCurrentCol(int newCurrentCol);
    bool getIsDragging() const;
};

#endif
