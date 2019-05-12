#include "keyinstance.h"

int KeyInstance::ID;

KeyInstance::KeyInstance(const Key & model, const qreal & x, const qreal & y) : QRectF(x-8,y-8,16,16), model(model){
    KeyInstance::ID++;
    id = KeyInstance::ID;
}

KeyInstance::KeyInstance(const Key &model, const QPointF & point) : QRectF(point.x()-8,point.y()-8,16,16),
    model(model)
{
    KeyInstance::ID++;
    id = KeyInstance::ID;
}

int KeyInstance::getId()
{
    return id;
}
