#ifndef ROOMCANVAS_H
#define ROOMCANVAS_H

#include "canvas.h"
#include "room.h"

#include <QWidget>



class RoomCanvas : public QWidget
{
    Q_OBJECT
public:
    RoomCanvas(Canvas * design);
    void toJson(QJsonObject & json) const;
    void fromJson(const QJsonObject & json);

protected:
    void showEvent(QShowEvent* ) override;
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool isRoomOverlapping(const Room & room);
    void deleteRoomsAt(const QPointF & point);
private:
    Canvas *design;
    QList<Room> roomList;
    QPointF * start = nullptr;
    QPointF * end = nullptr;
};

#endif // ROOMCANVAS_H
