#include "room.h"
#include "canvas.h"

int16_t Room::ID = 0;

Room::Room(QRectF &rect, QString name, bool exportRoom) : QRectF(rect), name(name), exportRoom(exportRoom)
{
    id = Room::ID++;
    if(this->name.isEmpty())
        this->name = "new_room_" + QString::number(id);
}

Room::Room(const QJsonObject &json)
{

    if(json.contains("ID") && json["ID"].isDouble())
        id = json["ID"].toInt();
    else
        throw std::runtime_error("no id for room in jsonObject");

    if(json.contains("name") && json["name"].isString())
        name = json["name"].toString();
    else
        throw std::runtime_error("no name for room in jsonObject");

    QPointF point1;
    if(json.contains("point1") && json["point1"].isString()){
        QRegExp regex(POINTSPLITREGEX);
        QStringList splitted = json["point1"].toString().split(regex, QString::SplitBehavior::SkipEmptyParts);
        if(splitted.size() < 2) // if there is less than 2 doubles, it ain't no point pal!
            throw std::runtime_error("error while parsing point: "+json["point1"].toString().toUtf8()); // we'll panic
        qreal x, y;
        x = splitted[0].toDouble();
        y = splitted[1].toDouble();
        point1 = QPointF(x,y);
    }
    else
        throw std::runtime_error("no point1 for room in jsonObject");

    QPointF point2;
    if(json.contains("point2") && json["point2"].isString()){
        QRegExp regex(POINTSPLITREGEX);
        QStringList splitted = json["point2"].toString().split(regex, QString::SplitBehavior::SkipEmptyParts);
        if(splitted.size() < 2) // if there is less than 2 doubles, it ain't no point pal!
            throw std::runtime_error("error while parsing point: "+json["point2"].toString().toUtf8()); // we'll panic
        qreal x, y;
        x = splitted[0].toDouble();
        y = splitted[1].toDouble();
        point2 = QPointF(x,y);
    }
    else
        throw std::runtime_error("no point1 for room in jsonObject");

    if(point1.isNull() || point2.isNull())
        throw std::runtime_error("invalid point for room in jsonObject");
    this->setCoords(point1.x(), point1.y(), point2.x(), point2.y());

    if(json.contains("export") && json["export"].isBool())
        this->exportRoom = json["export"].toBool();
    else
        this->exportRoom = false;
}

Room &Room::invalidRoom()
{
    static Room output(-1);
    return output;
}

QString Room::getName() const
{
    return name;
}

void Room::setName(const QString &value)
{
    name = value;
}

int32_t Room::getId() const
{
    return id;
}

QJsonObject Room::toJson() const
{
    QJsonObject output;
    output.insert("ID", this->id);
    output.insert("name", this->name);
    qreal x1, x2, y1, y2;
    getCoords(&x1, &y1, &x2, &y2);
    output.insert("point1", QString(POINTFORMAT).arg(x1).arg(y1));
    output.insert("point2", QString(POINTFORMAT).arg(x2).arg(y2));
    output.insert("export", exportRoom);
    return output;
}

bool Room::isValid() const
{
    return (id >= 0);
}

bool Room::isExportable() const
{
    return exportRoom;
}

void Room::setExportable(bool exp)
{
    exportRoom = exp;
}

Room::Room(int id)
{
    this->id = id;
}

/*
void Room::fromJson(const QJsonObject &json)
{
}
*/
