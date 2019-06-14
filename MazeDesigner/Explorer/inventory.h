#ifndef INVENTORY_H
#define INVENTORY_H

#include <QList>
#include "../MazeDesignerGUI/keyinstance.h"
#include "../ConditionParser/condition.h"

class Inventory
{
public:
    Inventory();
    Inventory(const Inventory & other);
    Inventory & collect(KeyInstance * key);
    Inventory & collect(const QList<KeyInstance*> & keys);
    Inventory & spend(const QString & keyModel, int num = 1);
    Inventory & spend(const QList<QString> & keyModels);
    Inventory & spend(const int & id, int num = 1);
    Inventory & spend(const QList<int> & ids);
    Inventory & spend(Condition::Cost & costs);
    bool canAfford(Condition::Cost & costs);
    bool operator ==(const Inventory & other) const;
    bool operator !=(const Inventory & other) const;
    bool contains(const int & keyModel, int num = 1);
    bool contains(const QList<int> & keyModels);
    bool contains(const QString & keyModel, int num = 1);
    bool contains(const QList<QString> & keyModels);
    int numberOfConsumables() const;
    int numberOfPowerUps() const;
    int length() const;
    QString toString() const;
    Inventory & operator =(const Inventory & other);

private:
    void updateCounts(const KeyInstance * key, int number = 1);
    QList<KeyInstance*> items;
    int nKeys = 0;
    int nPows = 0;
};

#endif // INVENTORY_H
