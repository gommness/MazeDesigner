#include "roomcanvas.h"

RoomCanvas::RoomCanvas(Canvas &design) : design(design) {}

void RoomCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QColor(0,0,0));
    painter.setRenderHint(QPainter::Antialiasing);
    for(auto room = roomList.begin(); room != roomList.end(); room++)
        painter.drawRect(*room);

    delete start;
    delete end;
    start = end = nullptr;
}

void RoomCanvas::mousePressEvent(QMouseEvent *event)
{
    delete start;
    start = new QPoint(design.getGrid().nearestPoint(event->pos()));
}

void RoomCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    delete end;
    end = new QPoint(design.getGrid().nearestPoint(event->pos()));
    if(start != end){
        Room room(QRect(*start, *end), "newRoom");
        if(!isRoomOverlapping(room)){
            roomList.append(room);
            update();
        }
    }
}

bool RoomCanvas::isRoomOverlapping(const Room &room)
{
    for(auto r = roomList.begin(); r != roomList.end(); r++){
        QRect rect(r->intersected(room));
        if(rect.width() > 1 && rect.height() > 1) // both larger than 1 to avoid considering "lines" or "edges" a invalid intersection
            return true;
    }
    return false;
}
