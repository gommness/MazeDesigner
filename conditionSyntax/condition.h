#ifndef CONDITION_H
#define CONDITION_H

#include <QString>
#include <QMap>
#include "key.h"

class Condition
{
public:
    Condition();
    virtual ~Condition();
    virtual bool validate();
    virtual void setNameSpace(QMap<QString, Key> *map);
    virtual QMap<QString, Key> getNameSpace();
    virtual QString toString();
private:
};

#endif // CONDITION_H
