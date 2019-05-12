#include "instancecanvas.h"
#include <QApplication>

InstanceCanvas::InstanceCanvas(Canvas * design, KeyListWidget * list) : design(design), keyList(list) {}

void InstanceCanvas::writeJson(QJsonObject &json) const
{

}

void InstanceCanvas::readJson(const QJsonObject &json)
{

}

void InstanceCanvas::showEvent(QShowEvent *)
{
    design->show();
}

void InstanceCanvas::paintEvent(QPaintEvent *event)
{
    design->update(); // we draw our design, then we'll drow aurselves
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,50,255, 128));
    pen.setWidth(4);
    painter.setPen(pen); //green color
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBackgroundMode(Qt::BGMode::TransparentMode);
    painter.scale(design->grid.getScale(), design->grid.getScale());

    for(auto door = doors.begin(); door != doors.end(); door++)
        painter.drawLine((*door)->translated(design->grid.getOffset()));

    for(auto key = keys.begin(); key != keys.end(); key++){
        painter.drawImage((*key)->translated(design->grid.getOffset()),QImage("../key.png")); // I hope this does not blow up
        // TODO draw different images whether the key is power up or not
    }

    if(startToken != nullptr)
        painter.drawEllipse(startToken->translated(design->grid.getOffset()));
}

void InstanceCanvas::mousePressEvent(QMouseEvent *event)
{
    QPointF * point = new QPointF(design->grid.nearestPoint(event->pos(), design->grid.getSize()/2));
    design->grid.mousePressEventHandler(event);
    if(event->buttons() & Qt::LeftButton){
        if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){
            createStartPoint(point);
            delete point;
        } else if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
            startCreatingDoor(point);
        } else {
            createKeyInstance(point);
            delete point;
        }
    } else if(event->button() & Qt::RightButton){
        destroyAt(point);
        delete point;
    }
    update();
}

void InstanceCanvas::mouseMoveEvent(QMouseEvent *event)
{
    design->grid.mouseMoveEventHandler(event);
}

void InstanceCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF end(design->grid.nearestPoint(event->pos()));
    design->grid.mouseReleaseEventHandler(event);
    if(start != nullptr){
        QLineF line(*start, end);
        doors.append(new DoorInstance(line));
    }
    delete start;
    start = nullptr;
}

void InstanceCanvas::wheelEvent(QWheelEvent *event)
{
    design->grid.wheelEvent(event);
}

void InstanceCanvas::createStartPoint(const QPointF *point)
{
    delete startToken;
    startToken = new QRectF(point->x()-8, point->y()-8, 16, 16);
}

void InstanceCanvas::startCreatingDoor(QPointF *point)
{
    start = point;
}

void InstanceCanvas::createKeyInstance(const QPointF *point)
{
   const Key &model = keyList->selectedKey();
   if(model.isValid())
       keys.append(new KeyInstance(model, point->x(), point->y()));
}

void InstanceCanvas::destroyAt(const QPointF *point)
{
    if(startToken->contains(*point)){
        delete startToken;
        startToken = nullptr;
    }
    for(int i = keys.size()-1; i >= 0; i--){
        if(keys[i]->contains(*point)){
            keys.removeAt(i);
        }
    }
    for(int i = doors.size()-1; i >= 0; i--){
        if(doors[i]->contains(*point)){
            doors.removeAt(i);
        }
    }
}
