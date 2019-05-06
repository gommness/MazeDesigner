#include "roomcanvas.h"
#include <QDebug>

RoomCanvas::RoomCanvas(Canvas *design) {
    this->design = design;
    design->show();
}

void RoomCanvas::toJson(QJsonObject & json) const
{

}

void RoomCanvas::fromJson(const QJsonObject &json)
{

}

void RoomCanvas::showEvent(QShowEvent *)
{
    design->show();
}

void RoomCanvas::paintEvent(QPaintEvent *)
{
    design->update(); // we draw our design, then we'll drow aurselves
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,255,33, 128));
    pen.setWidth(4);
    painter.setPen(pen); //green color
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBackgroundMode(Qt::BGMode::TransparentMode);
    painter.scale(design->grid.getScale(), design->grid.getScale());
    for(auto room = roomList.begin(); room != roomList.end(); room++)
        painter.drawRect(room->translated(design->grid.getOffset()));

    /*
    delete start;
    delete end;
    start = end = nullptr;
    */
}

void RoomCanvas::mousePressEvent(QMouseEvent *event)
{
    design->grid.mousePressEventHandler(event);
    delete start;
    if(event->button() & Qt::LeftButton) // left click
        start = new QPointF(design->grid.nearestPoint(event->pos()));
    else if(event->button() & Qt::RightButton){ // right click
        deleteRoomsAt(event->pos());
        start = nullptr;
    }
}

void RoomCanvas::mouseMoveEvent(QMouseEvent *event)
{
    design->grid.mouseMoveEventHandler(event);
}

void RoomCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    design->grid.mouseReleaseEventHandler(event);
    if(start == nullptr)
        return;
    delete end;
    end = new QPointF(design->grid.nearestPoint(event->pos()));
    if(start != end){
        QRectF rect(*start, *end);
        Room room(rect);
        if(!isRoomOverlapping(room)){
            roomList.append(room);
            update();
        }
    }
    delete end;
    delete start;
    end = start = nullptr;
}

void RoomCanvas::wheelEvent(QWheelEvent *event)
{
    design->grid.wheelEvent(event);
}

bool RoomCanvas::isRoomOverlapping(const Room &room)
{
    for(auto r = roomList.begin(); r != roomList.end(); r++){
        QRectF rect(r->intersected(room));
        if(rect.width() > 1 && rect.height() > 1) // both larger than 1 to avoid considering "lines" or "edges" a invalid intersection
            return true;
    }
    return false;
}

void RoomCanvas::deleteRoomsAt(const QPointF &point)
{
    bool flag = false; // will check if an update on the UI os necessary
    for(int i = roomList.length()-1; i >= 0; i--){
        if(roomList[i].contains(point)){
            flag = true;
            roomList.removeAt(i);
        }
    }
    if(flag)
        update();
}
