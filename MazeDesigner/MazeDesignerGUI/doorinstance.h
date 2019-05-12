#ifndef DOORINSTANCE_H
#define DOORINSTANCE_H

#include <QLineF>
#include <QPolygonF>
#include <QJsonObject>
#include "../Common/common.h"

class DoorInstance : public QLineF
{
public:
    DoorInstance(QLineF & line);
    bool contains(const QPointF & point) const;
    static DoorInstance fromJson(const QJsonObject & json);
    QJsonObject toJson() const;

private:
    DoorInstance(int id, QLineF & line);
    //TODO add the condition to the door
    static int ID;
    int id;
    QPolygonF area;
friend class InstanceCanvas;
};

#endif // DOORINSTANCE_H
