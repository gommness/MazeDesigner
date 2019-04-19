#ifndef ROOMCANVAS_H
#define ROOMCANVAS_H

#include "canvas.h"
#include "room.h"

#include <QWidget>



class RoomCanvas : QWidget
{
    Q_OBJECT
public:
    RoomCanvas(Canvas & design);

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool isRoomOverlapping(const Room & room);

private:
    Canvas &design;
    QList<Room> roomList;
    QPoint * start = nullptr;
    QPoint * end = nullptr;
};

#endif // ROOMCANVAS_H
