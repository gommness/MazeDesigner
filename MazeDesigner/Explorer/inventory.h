#ifndef INVENTORY_H
#define INVENTORY_H

#include <QList>
#include "../MazeDesignerGUI/keyinstance.h"

class Inventory
{
public:
    Inventory();
    Inventory(const Inventory & other);
    Inventory & collect(KeyInstance * key);
    Inventory & collect(const QList<KeyInstance*> & keys);
    Inventory & spend(const QString & keyModel);
    Inventory & spend(const QList<QString> & keyModels);
    Inventory & spend(const int & id);
    Inventory & spend(const QList<int> & ids);
    bool contains(const int & keyModel);
    bool contains(const QList<int> & keyModels);
    bool contains(const QString & keyModel);
    bool contains(const QList<QString> & keyModels);
    QString toString() const;
    Inventory & operator =(const Inventory & other);

private:
    QList<KeyInstance*> items;
};

#endif // INVENTORY_H
