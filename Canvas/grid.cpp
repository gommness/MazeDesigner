#include "grid.h"
#include <QPainter>
#include <QDebug>

Grid::Grid(QWidget * parent) : QWidget(parent)
{
    resize(parent->size());
}

Grid::Grid(const Grid &grid) : Grid(grid.parentWidget()) {}

QPoint Grid::nearestPoint(const QPoint &point) const
{
    // a point somewhere in the space will be inside a grid cell. That is, inside of a quad whose corners
    // are in-grid. By doing simple integer division and then multiplying by the size of a grid cell, we can
    // get the x and y coords of the top-left corner of said cell.
    int x = size*(point.x()/size);
    int y = size*(point.y()/size);
    // then we load the 4 points of the grid that make said quad into a list
    QList<QPoint> list;
    list.append(QPoint(x, y));
    list.append(QPoint(x, y+size));
    list.append(QPoint(x+size, y));
    list.append(QPoint(x+size, y+size));
    // and finally perform the classic iterative algorithm to find the nearest one
    double minDist = pointDistance(point, list[0]);
    QPoint output = list[0];
    for(int i = 1; i < list.length(); i++){
        double dist = pointDistance(point, list[i]);
        if(dist < minDist){
            output = list[i];
            minDist = dist;
        }
    }
    return output;
}

double Grid::pointDistance(const QPoint &p1, const QPoint &p2)
{
    return QLineF(p1, p2).length();
}

int8_t Grid::getSize() const
{
    return size;
}

void Grid::setSize(const int8_t &value)
{
    if(size > 0)
        size = value;
}

void Grid::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(200,200,200, 128)); // light grey and semi-transparent
    painter.setRenderHint(QPainter::Antialiasing);
    for(int x = 0; x < width(); x+=size)
        painter.drawLine(x, 0, x, height());
    for(int y = 0; y < width(); y+=size)
        painter.drawLine(0, y, width(), y);
}
