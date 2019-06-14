#include "inventory.h"

Inventory::Inventory()
{

}

Inventory::Inventory(const Inventory &other)
{
    for(auto item = other.items.begin(); item != other.items.end(); item++){
        items.append(new KeyInstance(**item));
    }
}

Inventory &Inventory::collect(KeyInstance *key)
{
    updateCounts(key);
    items.append(key);
    return *this;
}

Inventory &Inventory::collect(const QList<KeyInstance*> &keys)
{
    for(auto k = keys.begin(); k != keys.end(); k++)
        updateCounts(*k);
    items.append(keys);
    return *this;
}

Inventory &Inventory::spend(const QString &keyModel, int num)
{
    if(num <= 0)
        return *this;
    for(int i = 0; i < items.length(); i++){
        if(items[i]->getModel().getName() == keyModel){
            updateCounts(items[i]);
            items.removeAt(i);
            num--;
            if(num == 0)
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

Inventory &Inventory::spend(const int &id, int num)
{
    if(num <= 0)
        return *this;
    for(int i = 0; i < items.length(); i++){
        updateCounts(items[i]);
        if(items[i]->getModel().getId() == id){
            items.removeAt(i);
            num--;
            if(num == 0)
                return *this;
        }
    }
    return *this;
}

Inventory &Inventory::spend(const QList<int> &ids)
{
    for(auto key = ids.begin(); key != ids.end(); key++){
        spend(*key);
    }
    return *this;
}

Inventory &Inventory::spend(Condition::Cost &costs)
{
    return this->spend(costs.first->getId(), static_cast<int>(costs.second));
}

bool Inventory::canAfford(Condition::Cost &costs)
{
    return this->contains(costs.first->getId(), static_cast<int>(costs.second));
}

bool Inventory::operator ==(const Inventory &other) const
{
    return this->items == other.items;
}

bool Inventory::operator !=(const Inventory &other) const
{
    return !(*this == other);
}

bool Inventory::contains(const int &keyModel, int num)
{
    if(num <= 0)
        return true;
    for(auto key = items.begin(); key != items.end(); key++){
        if((*key)->getModel().getId() == keyModel){
            num--;
            if(num == 0)
                return true;
        }
    }
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

bool Inventory::contains(const QString &keyModel, int num)
{
    if(num <= 0)
        return true;
    for(auto key = items.begin(); key != items.end(); key++){
        if((*key)->getModel().getName() == keyModel){
            num--;
            if(num == 0)
                return true;
        }
    }
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

int Inventory::numberOfConsumables() const
{
    return nKeys;
}

int Inventory::numberOfPowerUps() const
{
    return nPows;
}

int Inventory::length() const
{
    return items.length();
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
    this->nKeys = other.nKeys;
    this->nPows = other.nPows;
    return *this;
}

void Inventory::updateCounts(const KeyInstance *key, int number)
{
    if(key->getModel().isPowerUp())
        nPows += number;
    else
        nKeys += number;
}
