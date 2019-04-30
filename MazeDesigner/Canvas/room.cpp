#include "room.h"

int16_t Room::ID = 0;

Room::Room(QRect &rect, QString name) : QRect(rect), name(name)
{
    id = Room::ID++;
    if(name.isEmpty())
        name = "new_room" + QString(id);
}

QString Room::getName() const
{
    return name;
}

void Room::setName(const QString &value)
{
    name = value;
}

int16_t Room::getId() const
{
    return id;
}
