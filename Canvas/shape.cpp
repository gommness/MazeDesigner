#include "shape.h"
#include <QSet>

Shape Shape::subtracted(const QPolygon &other)
{

}

void Shape::translate(int dx, int dy)
{
    this->QPolygon::translate(dx, dy);
    for(int i = 0; i < holes.length(); i++){
        holes[i].translate(dx, dy);
    }
}

Shape Shape::united(const QPolygon & other)
{
    QList<QPolygon> selfHoles(this->holes);
    this->QPolygon::operator=(other);
    this->holes = selfHoles;
    return *this;
}

Shape Shape::united(const Shape &other)
{
    QList<QPolygon> selfHoles(this->holes);
    QList<QPolygon> otherHoles(other.holes);
    *this = this->QPolygon::united(other);

    QList<QPolygon> holes;
    for(int i = 0; i < selfHoles.length(); i++){
        for(int j = 0; j < otherHoles.length(); j++){
            QPolygon aux = selfHoles[i].united(otherHoles[j]);
            if(!holes.contains(aux))
                holes.append(aux);
            //holes.insert(selfHoles[i].united(otherHoles[j])); // This far from optimal BUT IDK D:
        }
    }
    this->holes = holes;
    return *this;
}

Shape &Shape::operator=(const QPolygon &other)
{
    this->QPolygon::operator=(other);
    return *this;
}
