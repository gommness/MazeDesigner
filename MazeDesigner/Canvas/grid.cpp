#include "grid.h"
#include <QPainter>
#include <QMouseEvent>
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
    if(output.x() % size != 0){
        if(output.x() % size > size/2)
            output.setX(output.x() + size - (output.x()%size));
        else
            output.setX(output.x() - output.x()%size);
    }
    if(output.y() % size != 0){
        if(output.y() % size > size/2)
            output.setY(output.y() + size - (output.y()%size));
        else
            output.setY(output.y() - output.y()%size);
    }
    return output;
}

double Grid::pointDistance(const QPoint &p1, const QPoint &p2)
{
    return QLineF(p1, p2).length();
}

void Grid::updateOffset(const QPoint *from, const QPoint *to)
{
    hOffset += to->x() - from->x();
    vOffset += to->y() - from->y();
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

void Grid::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setPen(QColor(200,200,200, 128)); // light grey and semi-transparent
    painter.setRenderHint(QPainter::Antialiasing);
    for(int x = hOffset % size; x < width(); x+=size)
        painter.drawLine(x, 0, x, height());
    for(int y = vOffset & size; y < width(); y+=size)
        painter.drawLine(0, y, width(), y);
}

///README: Current problem with the drag mechanic is that a mouseMoveEvent does not know which button triggered it
/// which means that if we click with middle button, move the cursor and release middle button, we'd only know
/// that this action was performed helding the middle button during the release and press events
/// furthermore, if we clicked 2 different mouse buttons and released them in different order, bad things could (and will) happen
///
/// possible solutions:
///     1. state machine: the cheapest shit we could think of. Will probably be vulnerable to sigsevs if the order of clicks
///        is not what we expect.
///     2. EventFilters? Not wure how these could help us, but we might as well give them a try
///     3. Mouse attention grab or mouse tracking. If we detected the middle click, maybe by using the mouse tracking stuff
///        we could detect 100% when the middle is released (?)
///     4. panic attack: just lay down and cry for a couple of hours until a solution comes to mind
void Grid::mouseMoveEventHandler(QMouseEvent *event)
{
    qDebug() << "event handled!!";
    if(current == nullptr){
        qDebug() << ":(";
        return;
    }
    else if(event->button() & Qt::MiddleButton){ // the mouse wheel is pressed
        qDebug() << "middle click!";
        delete previous;
        previous = current;
        current = new QPoint(event->pos());
        updateOffset(previous, current);
    } else {
        qDebug() << "button: " << event->button();
    }
}

void Grid::mousePressEventHandler(QMouseEvent *event)
{
    delete current;
    current = new QPoint(event->pos());
}

void Grid::mouseReleaseEventHandler(QMouseEvent *)
{
    delete current;
    delete previous;
    current = previous = nullptr;
}

