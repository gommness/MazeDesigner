#ifndef INSTANCECANVAS_H
#define INSTANCECANVAS_H

#include <QWidget>
#include "../Canvas/canvas.h"
#include "keyinstance.h"
#include "doorinstance.h"
#include "../Keys/keylistwidget.h"

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
    void createStartPoint(const QPointF *point);
    void startCreatingDoor(QPointF *point);
    void createKeyInstance(const QPointF *point);
    void destroyAt(const QPointF *point);

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
