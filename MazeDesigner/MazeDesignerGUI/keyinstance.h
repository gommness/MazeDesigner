#ifndef KEYINSTANCE_H
#define KEYINSTANCE_H

#include <QPointF>
#include <QRectF>
#include "../Keys/key.h"
#include "../Keys/keyrepository.h"
#include "../Common/common.h"

class KeyInstance : public QRectF
{
public:
    KeyInstance(const Key & model, const qreal & x, const qreal & y);
    KeyInstance(const Key & model, const QPointF & point);
    static KeyInstance fromJson(const QJsonObject & json, const KeyRepository & repo);
    QJsonObject toJson();
    const Key model;
    int getId();
private:
    KeyInstance(int id, const Key & model, const qreal & x, const qreal & y);
    KeyInstance(int id, const Key & model, const QPointF & point);
    static int ID;
    int id;
friend class InstanceCanvas;
};

#endif // KEYINSTANCE_H
