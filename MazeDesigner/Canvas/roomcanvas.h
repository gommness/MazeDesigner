#ifndef ROOMCANVAS_H
#define ROOMCANVAS_H

#include "canvas.h"
#include "room.h"

#include <QWidget>

#define JSONROOMKEY "rooms"

class RoomCanvas : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief RoomCanvas constructs the canvas to store the rooms.
     * @param design the underlying design
     */
    RoomCanvas(Canvas * design);
    /**
     * @brief writeJson writes the information of the rooms into a json. Essentially serializing itself
     * @param json the json to write into
     */
    void writeJson(QJsonObject & json) const;
    /**
     * @brief readJson initializes its data from a json object
     * @param json the json object holding the representation of the roomCanvas
     */
    void readJson(const QJsonObject & json);
    /**
     * @brief minimumSizeHint
     * @return
     */
    QSize minimumSizeHint() const override;
    /**
     * @brief sizeHint
     * @return
     */
    QSize sizeHint() const override;

    QList<Room> & getRooms();

signals:
    void roomCreated(const Room & room);
    void roomDestroyed(const Room & room);
    void roomSelected(const Room & room);
protected:
    /**
     * @brief showEvent performs the show event
     */
    void showEvent(QShowEvent* ) override;
    /**
     * @brief paintEvent draws the rooms over the underlying design and grid
     * @param event
     */
    void paintEvent(QPaintEvent * event) override;
    /**
     * @brief mousePressEvent handles the mouse pressing input from the user. Starting the creation of a new room if it is
     * a left click and destroying whatever room is under the cursor if it was a right click
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent handles the mouse moving event
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent handles the mouse releasing event, ending the creation of a new room
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * @brief wheelEvent handles the scrolling input from the user
     * @param event
     */
    void wheelEvent(QWheelEvent *event) override;
    Room * roomAt(const QPointF & point);
    /**
     * @brief isRoomOverlapping checks whether a room is overlapping with another room
     * @param room the room to check for overlaps
     * @return true if there is another room overlaping with the given room
     */
    bool isRoomOverlapping(const Room & room);
    /**
     * @brief deleteRoomsAt destroys all rooms that contains the given point
     * @param point the point that will select which rooms will be destroyed
     */
    void deleteRoomsAt(const QPointF & point);
private:
    Canvas *design;
    QList<Room> roomList;
    Room * selected = nullptr;
    QPointF * start = nullptr;
    QPointF * end = nullptr;

    friend class Exporter;
};

#endif // ROOMCANVAS_H

