#ifndef INSTANCECANVAS_H
#define INSTANCECANVAS_H

#include <QWidget>
#include "../Canvas/canvas.h"
#include "keyinstance.h"
#include "doorinstance.h"
#include "../Keys/keylistwidget.h"

#define JSONINSTANCESKEY "instances"
#define JSONINSTANCESKEY_MAXID "key-instance-max-id"
#define JSONINSTANCESKEYSKEY "key-instances"
#define JSONINSTANCESDOOR_MAXID "door-instance-max-id"
#define JSONINSTANCESDOORSKEY "door-instances"
#define JSONINSTANCESTARTTOKENKEY "start-token-instance"

class InstanceCanvas : public QWidget
{
    Q_OBJECT
signals:
    void select(const SelectableInstance & inst);
    void selectDoor(DoorInstance & door);
    void clearSelection();
public:
    /**
     * @brief InstanceCanvas creates a Canvas for placing the instances. A base design canvas and the widget containing the list
     * of keys is needed
     * @param design the underlying design over which instances will be created
     * @param keyList the widget holding the list of keys
     */
    InstanceCanvas(Canvas * design, KeyListWidget * keyList);
    /**
     * @brief writeJson given a jsonObject, this method writes in it all the information needed to reconstruct itself.
     * essentially serializing itself
     * @param json the json to write into
     */
    void writeJson(QJsonObject & json) const;
    /**
     * @brief readJson initializes all data from a json object
     * @param json the json object that holds all the necessary information
     */
    void readJson(const QJsonObject & json);

protected:
    /**
     * @brief showEvent handles the show event
     */
    void showEvent(QShowEvent* ) override;
    /**
     * @brief paintEvent will draw all instances of keys, doors and the startToken. while also drawing the
     * underlying canvas and grid
     * @param event
     */
    void paintEvent(QPaintEvent * event) override;
    /**
     * @brief mousePressEvent handles the mouse inptu from the user. if it is a right click, it will perform a
     * destroy action, if it is a left click it will be a create action
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent handles the event of the movement of the mouse
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent handles the event of the release of a mouse button. If the object
     * was in the middle of the creation of a door, it will finish said creation.
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * @brief wheelEvent handles the user's mouse input of scrolling
     * @param event
     */
    void wheelEvent(QWheelEvent *event) override;
    /**
     * @brief createStartPoint creates the startToken instance at a given point
     * @param point the point in space where the startToken will be created
     */
    void createStartPoint(QPointF &point);
    /**
     * @brief startCreatingDoor starts the creation of a door instance. Stores the point passed by pointer in memory
     * @param point the point where the door will start its creation
     */
    void startCreatingDoor(QPointF *point);
    /**
     * @brief createKeyInstance creates a key instance. Which key will be created depends on the selected key in the
     * keyListWidget.
     * @param point the point where the key will be created
     */
    void createKeyInstance(QPointF &point);
    /**
     * @brief destroyAt destroys all instances of keys, doors and startTokens whose area contains the given point
     * @param point the point to perform the destruction
     */
    void destroyAt(const QPointF *point);
    /**
     * @brief isPlaceEmptyForKey checks whether a place in space holds an instance of a key.
     * @param point the point of space to check for
     * @return true if there is a key in that place
     */
    bool isPlaceEmptyForKey(const QPointF &point);

    KeyInstance * keyAt(const QPointF &point);

    DoorInstance * doorAt(const QPointF &point);
    /**
     * @brief isPlaceEmptyForToken check whether a place in space holds an instance of the startToken
     * @param point the point of space to check for
     * @return true if there is a key in tat place
     */
    bool isPlaceEmptyForToken(const QPointF &point);

protected slots:
    void onKeyModelDeleted(const Key & model);
    void onSpaceDestroyed(const QPolygonF & poly);
private:
    Canvas * design;
    KeyListWidget * keyList;
    QList<KeyInstance*> keys;
    QList<DoorInstance*> doors;
    SelectableInstance * selected = nullptr;
    QRectF * startToken = nullptr;
    QPointF * start = nullptr;
    QPointF * end = nullptr;
    QList<SelectableInstance*> path;

    friend class DesignGraph;
};

#endif // INSTANCECANVAS_H
