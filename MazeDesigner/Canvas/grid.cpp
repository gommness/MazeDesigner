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
    double dSize = static_cast<double>(size);
    QPointF offset(hOffset, vOffset);
    // a point somewhere in the space will be inside a grid cell. That is, inside of a quad whose corners
    // are in-grid. By doing simple integer division and then multiplying by the size of a grid cell, we can
    // get the x and y coords of the top-left corner of said cell.
    /// {point.x()} is the on-screen coord. we have to undo the scaling
    /// {point.x/scale} is the logic coord once the transformation is undone
    /// {offset} is the amount of logic translation
    /// thus, {point.x/scale - offset} is the logic coord of the input having the offset taken into acount
    /// {qFloor({previousLine}/dSize)} would be the number of the selected cell
    /// {dSize*{qFloor(...)}} would be the top-left corner of the cell containing the selected point
    double x = dSize*(qFloor(((point.x())/scale-hOffset)/dSize));
    double y = dSize*(qFloor(((point.y())/scale-vOffset)/dSize));
    // thus, we now have x and y the logical coords of the top-left corner of the cell that contains the point
    // then we load the 4 points of the grid that make said cell into a list
    QList<QPointF> list;
    list.append(QPointF(x, y)); // top-left
    list.append(QPointF(x, y+dSize)); // bottom-left
    list.append(QPointF(x+dSize, y)); // top-right
    list.append(QPointF(x+dSize, y+dSize)); // bottom-right

    for(auto a = list.begin(); a != list.end(); a++)
        qDebug() << *a;
    // and finally perform the classic iterative algorithm to find the nearest one
    double minDist = pointDistance(point/scale-offset, list[0]); // we initialize a distance with the first corner
    //note that dist is calculated this way:
    /// second point would be an in-grid point. No need to worry too much
    /// first point is calculated as follows:
    ///     the out-of-grid point whose coords are NOT logical
    ///     inverted Scale its coords to make them logical
    ///     then substract the offset to undo the translation of the grid
    ///     and that is the logical point
    QPointF output = list[0]; // and initialize the output to the corresponding QPoint
    //qDebug() << "original point: " << point;
    //qDebug() << "x:" << x << "y:" << y;
    for(int i = 1; i < list.length(); i++){ // loop starts at 1 since the previous initialization was with index 0. No need to repeat
        double dist = pointDistance(point/scale-offset, list[i]); // calculate dist, same as before
        //qDebug() << "point"<<i<<list[i]<<"distance:"<<dist;
        if(dist < minDist){ // if new min dist is found, update output and minDist
            output = list[i];
            minDist = dist;
        }
    }
    /*
    // this obsolete code was done to force the elimination of the "lines" in the canvas. it is NOT needed
    // but I'll leave it here just in case it becomes useful at some point
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
    //qDebug() << "ouptut"<<output;
    return output;
}

QPointF Grid::centerOfCellAt(const QPointF &point) const
{
    QPointF output = this->nearestPoint(point);
    QPointF p = adapted(point);
    qreal halfSize = size/2;
    if(output.x() >= p.x()){ // snapped-point is right of point
        if(output.y() >= p.y()){ // snapped-point is below of point
            output.setX(output.x()-halfSize);
            output.setY(output.y()-halfSize);
        } else { // snapped-point is above of point
            output.setX(output.x()-halfSize);
            output.setY(output.y()+halfSize);
        }
    } else { // snapped-point is left of point
        if(output.y() >= p.y()){ // snapped-point is below of point
            output.setX(output.x()+halfSize);
            output.setY(output.y()-halfSize);
        } else { // snapped-point is above of point
            output.setX(output.x()+halfSize);
            output.setY(output.y()+halfSize);
        }
    }
    return output;
}

QPointF Grid::adapted(const QPointF &point) const
{
    return QPointF(point.x()/scale - hOffset, point.y()/scale - vOffset);
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
    QPoint numDegrees = event->angleDelta();

    int deltaY = 0;
    if (!numPixels.isNull()) {
        deltaY = numPixels.y();
    } else if (!numDegrees.isNull()) {
        deltaY = numDegrees.y();
    }
    //qDebug() << "numSteps: " << deltaY;
    if(deltaY > 0) { // wheel going upwards
        if (scale <= 2)
            scale *= 2;
    } else if(deltaY < 0){ // wheel going downwards
        if(scale >= 0.5)
            scale /= 2;
    }
    //qDebug() << "scale: "<< scale;
    update();
}

QSize Grid::sizeHint() const
{
    //return parentWidget()->sizeHint();
    return QSize(4000, 2000);
}

QPoint Grid::getOffset() const
{
    return QPoint(hOffset, vOffset);
}

qreal Grid::getScale() const
{
    return scale;
}

