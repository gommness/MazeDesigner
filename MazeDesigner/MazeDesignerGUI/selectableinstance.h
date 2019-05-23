#ifndef SELECTABLEINSTANCE_H
#define SELECTABLEINSTANCE_H

#include <QString>

class SelectableInstance
{
public:
    virtual QString instanceInfo() const = 0;
    virtual ~SelectableInstance() = 0;
};
#endif // SELECTABLEINSTANCE_H
