#include "roomcanvas.h"

RoomCanvas::RoomCanvas(Canvas *design) {
    this->design = design;
    design->show();
}

void RoomCanvas::showEvent(QShowEvent *)
{
    if(!design->isVisible())
        design->show();
}

void RoomCanvas::paintEvent(QPaintEvent *)
{
    design->update(); // we draw our design, then we'll drow aurselves
    QPainter painter(this);
    painter.setPen(QColor(0,255,33)); //green color
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBackgroundMode(Qt::BGMode::TransparentMode);
    for(auto room = roomList.begin(); room != roomList.end(); room++)
        painter.drawRect(*room);

    /*
    delete start;
    delete end;
    start = end = nullptr;
    */
}

void RoomCanvas::mousePressEvent(QMouseEvent *event)
{
    //delete start;
    start = new QPoint(design->grid.nearestPoint(event->pos()));
}

void RoomCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    end = new QPoint(design->grid.nearestPoint(event->pos()));
    if(start != end){
        QRect rect(*start, *end);
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

bool RoomCanvas::isRoomOverlapping(const Room &room)
{
    for(auto r = roomList.begin(); r != roomList.end(); r++){
        QRect rect(r->intersected(room));
        if(rect.width() > 1 && rect.height() > 1) // both larger than 1 to avoid considering "lines" or "edges" a invalid intersection
            return true;
    }
    return false;
}
