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

    for(auto door = doors.begin(); door != doors.end(); door++){
        painter.drawLine((*door)->translated(design->grid.getOffset()));
        painter.drawPolygon((*door)->area.translated(design->grid.getOffset()));
    }

    for(auto key = keys.begin(); key != keys.end(); key++){
        painter.drawImage((*key)->translated(design->grid.getOffset()),QImage("../key.png")); // I hope this does not blow up
        // TODO draw different images whether the key is power up or not
    }

    if(startToken != nullptr)
        painter.drawEllipse(startToken->translated(design->grid.getOffset()));
}

void InstanceCanvas::mousePressEvent(QMouseEvent *event)
{
    design->grid.mousePressEventHandler(event);
    if(event->buttons() & Qt::LeftButton){
        if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){
            QPointF point = design->grid.centerOfCellAt(event->pos());
            createStartPoint(&point);
        } else if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
            QPointF *point = new QPointF(design->grid.nearestPoint(event->pos()));
            startCreatingDoor(point);
        } else {
            QPointF point = design->grid.centerOfCellAt(event->pos());
            createKeyInstance(&point);
        }
    } else if(event->button() & Qt::RightButton){
        QPointF point(event->pos());
        destroyAt(&point);
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
    // some stackOverflow wizard told me that != works as xor for bool values as follows:
    // A xor B <=> !A != !B
    // the more you know
    if(start != nullptr &&
            (!(std::abs(start->x()-end.x()) < UMBRAL) != !(std::abs(start->y()-end.y()) < UMBRAL))){
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

void InstanceCanvas::createStartPoint(QPointF *point)
{
    delete startToken;
    startToken = new QRectF(point->x()-8, point->y()-8, 16, 16);
}

void InstanceCanvas::startCreatingDoor(QPointF *point)
{
    start = point;
}

void InstanceCanvas::createKeyInstance(QPointF *point)
{
   const Key &model = keyList->selectedKey();
   if(model.isValid())
       keys.append(new KeyInstance(model, point->x(), point->y()));
}

void InstanceCanvas::destroyAt(const QPointF *point)
{
    if(startToken != nullptr && startToken->contains(*point)){
        delete startToken;
        startToken = nullptr;
    }
    for(int i = keys.size()-1; i >= 0; i--){
        if(keys[i]->contains(*point)){
            delete keys[i];
            keys.removeAt(i);
        }
    }
    for(int i = doors.size()-1; i >= 0; i--){
        if(doors[i]->contains(*point)){
            delete doors[i];
            doors.removeAt(i);
        }
    }
}
