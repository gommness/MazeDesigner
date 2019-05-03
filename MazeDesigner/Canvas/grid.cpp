#include "grid.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>

Grid::Grid(QWidget * parent) : QWidget(parent)
{
    resize(parent->size());
}

Grid::Grid(const Grid &grid) : Grid(grid.parentWidget()) {}

QPointF Grid::nearestPoint(const QPointF &point) const
{
    /// problema: recibimos coordenadas reales del widget y queremos obtener las coordenadas virtuales
    /// de nuestro grid a las que se corresponden. Teniendo en cuenta que hay una transformación de escalado
    /// y de offset
    double dSize = static_cast<double>(size);
    qreal invertedS = dSize/scale;
    qreal directS = dSize*scale;
    QPointF offset(hOffset, vOffset);
    // a point somewhere in the space will be inside a grid cell. That is, inside of a quad whose corners
    // are in-grid. By doing simple integer division and then multiplying by the size of a grid cell, we can
    // get the x and y coords of the top-left corner of said cell.
    /// point.x() is the on-screen coord
    /// we have to invertedScale the coord (i.e. if the scale is 0.5 then the logic point would be double the distance farther away)
    /// offset is the amount of UNSCALED translation of the grid
    /// point.x - offset is the
    double x = dSize*(qFloor((point.x()/scale-hOffset)/dSize));
    double y = dSize*(qFloor((point.y()/scale-vOffset)/dSize));
    // then we load the 4 points of the grid that make said quad into a list
    QList<QPointF> list;
    list.append(QPointF(x, y));
    list.append(QPointF(x, y+dSize));
    list.append(QPointF(x+dSize, y));
    list.append(QPointF(x+dSize, y+dSize));
    // and finally perform the classic iterative algorithm to find the nearest one
    double minDist = pointDistance(point/scale-offset, list[0]);
    QPointF output = list[0];
    qDebug() << "original point: " << point;
    qDebug() << "x:" << x << "y:" << y;
    for(int i = 1; i < list.length(); i++){
        double dist = pointDistance(point/scale-offset, list[i]);
        qDebug() << "point"<<i<<list[i]<<"distance:"<<dist;
        if(dist < minDist){
            output = list[i];
            minDist = dist;
        }
    }
    /*
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
    */
    qDebug() << "ouptut"<<output;
    return output;
}

double Grid::pointDistance(const QPointF &p1, const QPointF &p2)
{
    return QLineF(p1, p2).length();
}

void Grid::updateOffset(const QPoint *from, const QPoint *to)
{
    hOffset += (to->x() - from->x())/scale;
    vOffset += (to->y() - from->y())/scale;
    update();
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
    painter.scale(scale, scale);
    painter.setPen(QColor(0,0,0, 128)); // light grey and semi-transparent
    painter.setRenderHint(QPainter::Antialiasing);
    double widthRatio = static_cast<double>(width())/scale;
    double heightRatio = static_cast<double>(height())/scale;
    for(int x = hOffset % size; x < widthRatio; x+=size)
        painter.drawLine(x, 0, x, qCeil(heightRatio));
    for(int y = vOffset % size; y < heightRatio; y+=size)
        painter.drawLine(0, y, qCeil(widthRatio), y);
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
    if(current == nullptr){
        return;
    }
    else { // the mouse wheel is pressed
        delete previous;
        previous = current;
        current = new QPoint(event->pos());
        updateOffset(previous, current);
    }
}

void Grid::mousePressEventHandler(QMouseEvent *event)
{
    delete current;
    if(event->button() & Qt::MiddleButton)
        current = new QPoint(event->pos());
    else
        current = nullptr;
}

void Grid::mouseReleaseEventHandler(QMouseEvent *event)
{
    if(event->button() & Qt::MiddleButton){
        delete current;
        delete previous;
        current = previous = nullptr;
    }
}

void Grid::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        //scrollWithPixels(numPixels);
    } else if (!numDegrees.isNull()) {
        double deltaY = static_cast<double>(numDegrees.y());
        qDebug() << "numSteps: " << deltaY;
        if(deltaY > 0) { // wheel going upwards
            if (scale <= 2)
                scale *= 2;
        } else if(deltaY < 0){ // wheel going downwards
            if(scale >= 0.5)
                scale /= 2;
        }
        qDebug() << "scale: "<< scale;
        update();
        //scrollWithDegrees(numSteps);
    }
}

QPoint Grid::getOffset() const
{
    return QPoint(hOffset, vOffset);
}

qreal Grid::getScale() const
{
    return scale;
}

