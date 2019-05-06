#ifndef INSTANCECANVAS_H
#define INSTANCECANVAS_H

#include <QWidget>
#include <QJsonObject>

class InstanceCanvas : QWidget
{
    class KeyInstance;
    class DoorInstance;
    Q_OBJECT
public:
    InstanceCanvas();
    ~InstanceCanvas() override;
    void toJson(QJsonObject & json) const;
    void fromJson(const QJsonObject & json);



private:
    // should doors and keys be pointers?
    QList<DoorInstance> doors;
    QList<KeyInstance> keys;
    QPointF * startToken;
};

#endif // INSTANCECANVAS_H
