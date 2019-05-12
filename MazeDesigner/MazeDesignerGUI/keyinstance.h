#ifndef KEYINSTANCE_H
#define KEYINSTANCE_H

#include <QPointF>
#include <QRectF>
#include "../Keys/key.h"

class KeyInstance : public QRectF
{
public:
    KeyInstance(const Key & model, const qreal & x, const qreal & y);
    KeyInstance(const Key & model, const QPointF & point);
    const Key & model;
    int getId();
private:
    static int ID;
    int id;
};

#endif // KEYINSTANCE_H
