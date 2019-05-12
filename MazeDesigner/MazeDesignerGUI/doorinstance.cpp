#include "doorinstance.h"

int DoorInstance::ID = 0;

DoorInstance::DoorInstance(QLineF &line) : QLineF (line)
{
    DoorInstance::ID++;
    id = DoorInstance::ID;
    QVector<QPointF> poly;
    QLineF normal = line.normalVector();
    normal.setLength(4);
    //construct the polygon surrounding the line
    poly.append(line.p1()-normal.p1());
    poly.append(line.p2()-normal.p1());
    poly.append(line.p2()-normal.p2());
    poly.append(line.p1()-normal.p2());
    area = QPolygonF(poly);
}

bool DoorInstance::contains(const QPointF &point) const
{
    return area.containsPoint(point, Qt::FillRule::OddEvenFill);
}

