#include "key.h"
#include <QDebug>

int Key::ID = 0;

Key::Key(const QString &name, const bool &value)
{
    Key::ID ++;
    if(name == ""){
        this->name = QString("new_key") + QString::number(Key::ID);
    } else {
        this->name = name;
    }
    id = Key::ID;
    this->powerUp = value;
}

Key::Key(const QJsonObject &json)
{
    if(json.contains("ID") && json["ID"].isDouble())
        id = json["ID"].toInt();
    else
        throw std::runtime_error("no id for key in jsonObject");

    if(json.contains("name") && json["name"].isString())
        name = json["name"].toString();
    else
        throw std::runtime_error("no name for key in jsonObject");

    if(json.contains("power-up") && json["power-up"].isBool())
        powerUp = json["power-up"].toBool();
    else
        throw std::runtime_error("no power-up value for key in jsonObject");
}

Key &Key::invalidKey()
{
    static Key output(-1);
    return output;
}

QString Key::getName() const
{
    return name;
}

int Key::getId() const
{
    return id;
}

bool Key::isValid() const
{
    return (id >= 0);
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

QJsonObject Key::toJson()
{
    QJsonObject output;
    output.insert("ID", id);
    output.insert("name", name);
    output.insert("power-up", powerUp);
    return output;
}

bool Key::operator ==(const Key &key) const
{
    return this->id == key.id;
}

Key::Key(int id)
{
    this->id = id;
}

