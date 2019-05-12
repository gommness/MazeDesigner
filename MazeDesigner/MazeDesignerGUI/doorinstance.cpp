#include "doorinstance.h"

int DoorInstance::ID = 0;

DoorInstance::DoorInstance(QLineF &line) : QLineF (line)
{
    DoorInstance::ID++;
    id = DoorInstance::ID;
    QVector<QPointF> poly;
    QLineF normal = line.normalVector();
    // normalVector, despite the name, returns a orthogonal line starting at the same point as line
    // thus, we need to translate that normalLines to the origin, maintaining the vector
    normal.setP1(QPointF(0,0));
    normal.setP2(normal.p2()-line.p1());
    // then adjust the length of the line for area purposes
    normal.setLength(4);
    // and finally construct the polygon surrounding the line
    poly.append(line.p1()+normal.p2());
    poly.append(line.p2()+normal.p2());
    normal.setLength(-4); // we invert the length here to capture the other side
    poly.append(line.p2()+normal.p2());
    poly.append(line.p1()+normal.p2());
    area = QPolygonF(poly);
}

bool DoorInstance::contains(const QPointF &point) const
{
    return area.containsPoint(point, Qt::FillRule::OddEvenFill);
}

