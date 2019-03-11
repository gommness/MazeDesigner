#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget (parent)
{
    transformed = false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
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

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QColor(0,0,0)); // black color
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(event->rect());
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
    if(start != end){
        QRect rect = QRect(*start, *end);
        update(rect);
        //QPainter painter(this);
        //painter.drawRect(rect);
        //QPaintEvent pEvent = QPaintEvent(rect);
        //paintEvent(&pEvent);
    }
}
