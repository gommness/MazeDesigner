#include "canvas.h"
#include <QPainter>
#include <QDebug>
#include <QLineF>

Canvas::Canvas(QWidget *parent) : QWidget (parent)
{
    transformed = false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    polyList = QList<QPolygon>();
    setGridSize(32);
    //painter.begin(this);
    //painter.setPen(QColor(0,0,0));
    //painter.setRenderHint(QPainter::Antialiasing);
    //qDebug() << "painter active? " << painter.isActive();
}

Canvas::~Canvas()
{
    delete start;
    delete end;
    start = end = nullptr;
}

QSize Canvas::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Canvas::sizeHint() const
{
    return QSize(400, 200);
}

void Canvas::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void Canvas::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

QString Canvas::toString() const
{
    QString output("");
    foreach(QPolygon poly, polyList){
        output += "polygon : { ";
        foreach(QPoint coord, poly){
            output += "Point: {x: "+QString::number(coord.x()) +" y: "+QString::number(coord.y()) +"} ";
        }
        output += " }\n";
    }
    return output;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);
    //painter.setPen(QColor(0,0,0)); // black color
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.begin(this);
    painter.setPen(QColor(0,0,0));
    painter.setRenderHint(QPainter::Antialiasing);
    // qDebug() << "painter active? " << painter.isActive();
    render();
    painter.end();
    //painter.drawRect(event->rect());
    delete start;
    delete end;
    start = end = nullptr;
}



void Canvas::mousePressEvent(QMouseEvent *event)
{
    delete start;
    start = new QPoint(nearestGridPoint(event->pos()));
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    delete end;
    end = new QPoint(nearestGridPoint(event->pos()));
    // qDebug() << "recieved mouse elease event";
    if(start != end){
        QRect rect = QRect(*start, *end);
        addPolygon(rect);
        update();
        //QPainter painter(this);
        //painter.drawRect(rect);
        //QPaintEvent pEvent = QPaintEvent(rect);
        //paintEvent(&pEvent);
    }
}

void Canvas::addPolygon(const QPolygon &other)
{
    //qDebug() << "adding polygon " << other.toList();
    for(auto poly = polyList.begin(); poly != polyList.end(); poly = poly++){
        if(poly->intersects(other)){
            *poly = poly->united(other);
            return;
        }
    }
    polyList.append(other);
}

void Canvas::render()
{
    //qDebug() << "is empty? " << this->polyList.isEmpty() << this->toString();
    foreach(QPolygon poly, polyList){
        painter.drawPolygon(poly);
    }
}

double pointDistance(const QPoint &p1, const QPoint &p2){
    return QLineF(p1, p2).length();
}

QPoint Canvas::nearestGridPoint(const QPoint &point) const
{
    // a point somewhere in the space will be inside a grid cell. That is, inside of a quad whose corners
    // are in-grid. By doing simple integer division and then multiplying by the size of a grid cell, we can
    // get the x and y coords of the top-left corner of said cell.
    int x = gridSize*(point.x()/gridSize);
    int y = gridSize*(point.y()/gridSize);
    QList<QPoint> list;
    list.append(QPoint(x, y));
    list.append(QPoint(x, y+gridSize));
    list.append(QPoint(x+gridSize, y));
    list.append(QPoint(x+gridSize, y+gridSize));
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

int8_t Canvas::getGridSize() const
{
    return gridSize;
}

void Canvas::setGridSize(const int8_t &value)
{
    if(value > 0 && polyList.isEmpty())
        gridSize = value;
}
