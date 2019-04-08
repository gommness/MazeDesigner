#include "room.h"

int16_t Room::ID = 0;

Room::Room(QRect rect, QString name) : QRect(rect), name(name)
{
    id = Room::ID++;
}

QString Room::getName() const
{
    return name;
}

void Room::setName(const QString &value)
{
    name = value;
}
