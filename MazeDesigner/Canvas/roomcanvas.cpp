#include "roomcanvas.h"
#include <QDebug>
#include <QJsonArray>

RoomCanvas::RoomCanvas(Canvas *design) {
    this->design = design;
    design->show();
}

void RoomCanvas::writeJson(QJsonObject & json) const
{
    QJsonArray rooms;
    for(auto room = roomList.begin(); room != roomList.end(); room++){
        rooms.append(room->toJson());
    }
    json.insert(JSONROOMKEY, rooms);
}

void RoomCanvas::readJson(const QJsonObject &json)
{
    if(json.contains(JSONROOMKEY) && json[JSONROOMKEY].isArray()){
        QJsonArray jsonArray = json[JSONROOMKEY].toArray();
        QList<Room> aux;
        for(auto jsonRoom = jsonArray.begin(); jsonRoom != jsonArray.end(); jsonRoom++){
            if(jsonRoom->isObject()){ // no else so that if there is a non-obj entry, we don't freak out
                aux.append(Room(jsonRoom->toObject()));
            }
        }
        //roomList.clear();
        roomList = aux;
    }
}

QSize RoomCanvas::minimumSizeHint() const
{
    return design->minimumSizeHint();
}

QSize RoomCanvas::sizeHint() const
{
    return design->sizeHint();
}

void RoomCanvas::showEvent(QShowEvent *)
{
    design->show();
    update();
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
    if(selected != nullptr){
        pen.setColor(QColor(255,255,0,128));
        pen.setWidth(6);
        painter.setPen(pen);
        painter.drawRect(selected->translated(design->grid.getOffset()));
    }

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
    if(event->button() & Qt::LeftButton){ // left click
        Room * room = roomAt(design->grid.adapted(event->pos()));
        if(room != nullptr){
            selected = room;
            emit roomSelected(*room);
            start = nullptr;
        } else {
            selected = nullptr;
            emit roomSelected(Room::invalidRoom());
            start = new QPointF(design->grid.nearestPoint(event->pos()));
        }
    }
    else if(event->button() & Qt::RightButton){ // right click
        deleteRoomsAt(design->grid.adapted(event->pos()));
        start = nullptr;
        selected = nullptr;
        emit roomSelected(Room::invalidRoom());
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
            selected = &roomList.last();
            emit roomCreated(room);
            emit roomSelected(room);
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

Room *RoomCanvas::roomAt(const QPointF &point)
{
    for(auto room = roomList.begin(); room != roomList.end(); room++)
        if(room->contains(point))
            return &(*room); // LOL I never thought I'd see this &(*(something))  as something necessary
    return nullptr;
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
            if(selected == &roomList[i]){
                selected = nullptr;
                emit roomSelected(Room::invalidRoom());
            }
            emit roomDestroyed(roomList[i]);
            roomList.removeAt(i);
        }
    }
    if(flag)
        update();
}

QList<Room> &RoomCanvas::getRooms()
{
    return roomList;
}
