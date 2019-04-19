#ifndef SHAPE_H
#define SHAPE_H

#include <QPolygon>

class Shape : public QPolygon
{
public:
    Shape subtracted(const QPolygon & other);
    void translate(int dx, int dy);
    Shape united(const QPolygon & other);
    Shape united(const Shape & other);
    Shape & operator=(const QPolygon & other);
private:
    QList<QPolygon> holes;
};

#endif // SHAPE_H
