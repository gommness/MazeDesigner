#include "canvas.h"
#include <QPainter>
#include <QDebug>

Canvas::Canvas(QWidget *parent) : QWidget (parent)
{
    transformed = false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    polyList = QList<QPolygon>();
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
    start = new QPoint(event->pos());
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    delete end;
    end = new QPoint(event->pos());
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
