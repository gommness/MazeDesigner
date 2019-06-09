#include "inventory.h"

Inventory::Inventory()
{

}

Inventory::Inventory(const Inventory &other)
{
    for(auto item = other.items.begin(); item != other.items.end(); item++){
        items.append(*item);
    }
}

Inventory &Inventory::collect(KeyInstance *key)
{
    items.append(key);
    return *this;
}

Inventory &Inventory::collect(const QList<KeyInstance*> &keys)
{
    items.append(keys);
    return *this;
}

Inventory &Inventory::spend(const QString &keyModel)
{
    for(int i = 0; i < items.length(); i++){
        if(items[i]->model.getName() == keyModel){
            items.removeAt(i);
            return *this;
        }
    }
    return *this;
}

Inventory &Inventory::spend(const QList<QString> &keyModels)
{
    for(auto key = keyModels.begin(); key != keyModels.end(); key++)
        spend(*key);
    return *this;
}

Inventory &Inventory::spend(const int &id)
{
    for(int i = 0; i < items.length(); i++){
        if(items[i]->model.getId() == id){
            items.removeAt(i);
            return *this;
        }
    }
    return *this;
}

Inventory &Inventory::spend(const QList<int> &ids)
{
    for(auto key = ids.begin(); key != ids.end(); key++)
        spend(*key);
    return *this;
}

bool Inventory::contains(const int &keyModel)
{
    for(auto key = items.begin(); key != items.end(); key++)
        if((*key)->model.getId() == keyModel)
            return true;
    return false;
}

bool Inventory::contains(const QList<int> &keyModels)
{
    int counter = keyModels.length();
    for(auto id = keyModels.begin(); id != keyModels.end(); id++){
        if(contains(*id))
            counter++;
        if(counter <= 0)
            return true;
    }
    return false;
}

bool Inventory::contains(const QString &keyModel)
{
    for(auto key = items.begin(); key != items.end(); key++)
        if((*key)->model.getName() == keyModel)
            return true;
    return false;
}

bool Inventory::contains(const QList<QString> &keyModels)
{
    int counter = keyModels.length();
    for(auto id = keyModels.begin(); id != keyModels.end(); id++){
        if(contains(*id))
            counter++;
        if(counter <= 0)
            return true;
    }
    return false;
}

QString Inventory::toString() const
{
    QString output = "[";
    for(auto key = items.begin(); key != items.end(); key++)
        output += (*key)->toString() + "    ";
    return output + "]";
}

Inventory &Inventory::operator =(const Inventory &other)
{
    this->items.clear();
    this->items.append(other.items);
}
