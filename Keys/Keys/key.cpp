#include "key.h"
#include <QDebug>

int Key::ID = 0;

Key::Key(const QString &name, const bool &value)
{
    if(name == ""){
        this->name = QString("new_key") + QString::number(Key::ID);
        Key::ID ++;
    } else {
        this->name = name;
    }
    this->powerUp = value;
}

QString Key::getName() const
{
    return name;
}

void Key::setName(const QString &name)
{
    //qDebug() << "calling " << __FUNCTION__ << " name: " << name;
    this->name = name;
}

bool Key::isPowerUp() const
{
    return powerUp;
}

void Key::setPowerUp(const bool &value)
{
    //qDebug() << "calling " << __FUNCTION__ << " value: " << value;
    this->powerUp = value;
}

QString Key::toString()
{
    return QString("keyname " + name + " power up: " + powerUp);
}
