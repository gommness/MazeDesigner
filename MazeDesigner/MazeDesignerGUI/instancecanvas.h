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
public:
    InstanceCanvas(Canvas * design, KeyListWidget * keyList);
    void writeJson(QJsonObject & json) const;
    void readJson(const QJsonObject & json);

protected:
    void showEvent(QShowEvent* ) override;
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void createStartPoint(QPointF &point);
    void startCreatingDoor(QPointF *point);
    void createKeyInstance(QPointF &point);
    void destroyAt(const QPointF *point);
    bool isPlaceEmptyForKey(const QPointF &point);
    bool isPlaceEmptyForToken(const QPointF &point);

private:
    Canvas * design;
    KeyListWidget * keyList;
    QList<KeyInstance*> keys;
    QList<DoorInstance*> doors;
    QRectF * startToken = nullptr;
    DoorInstance * selectedDoor = nullptr;
    QPointF * start = nullptr;
    QPointF * end = nullptr;
};

#endif // INSTANCECANVAS_H
