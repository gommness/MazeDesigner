#include "canvas.h"
#include <QPainter>
#include <QDebug>
#include <QLineF>

Canvas::Canvas(QWidget *parent) : QWidget (parent), grid(this)
{
    transformed = false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    polyList = QList<QPolygon>();
    grid.setSize(32);
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
    //grid.update();
    painter.begin(this);
    painter.setPen(QColor(0,0,0));
    painter.setRenderHint(QPainter::Antialiasing);
    render();
    painter.end();

    delete start;
    delete end;
    start = end = nullptr;
}



void Canvas::mousePressEvent(QMouseEvent *event)
{
    delete start;
    start = new QPoint(grid.nearestPoint(event->pos()));
}

/*
 * TODO draw rectangle that traces the movement of the mouse. Might need another "canvas" to do this
void Canvas::mouseMoveEvent(QMouseEvent *event){
    if(start != nullptr){
        QRect rect(*start, nearestGridPoint(event->pos()));
        update(rect.normalized());
    }
}
*/

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    delete end;
    end = new QPoint(grid.nearestPoint(event->pos()));
    // qDebug() << "recieved mouse elease event";
    if(start != end){
        QRect rect(*start, *end);
        addPolygon(rect);
        update();
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

const Grid Canvas::getGrid() const
{
    return Grid(grid);
}
