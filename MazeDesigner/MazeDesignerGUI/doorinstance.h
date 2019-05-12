#ifndef DOORINSTANCE_H
#define DOORINSTANCE_H

#include <QLineF>
#include <QPolygonF>

class DoorInstance : public QLineF
{
public:
    DoorInstance(QLineF & line);
    bool contains(const QPointF & point) const;

private:
    //TODO add the condition to the door
    static int ID;
    int id;
    QPolygonF area;
friend class InstanceCanvas;
};

#endif // DOORINSTANCE_H
