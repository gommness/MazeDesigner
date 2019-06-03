#ifndef SELECTABLEINSTANCE_H
#define SELECTABLEINSTANCE_H

#include <QString>
#include <QPolygonF>

class SelectableInstance
{
public:
    virtual QPolygonF boundPolygon() const = 0;
    virtual QString instanceInfo() const = 0;
    virtual ~SelectableInstance() = 0;
};
#endif // SELECTABLEINSTANCE_H
