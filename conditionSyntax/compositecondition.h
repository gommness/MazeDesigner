#ifndef COMPOSITECONDITION_H
#define COMPOSITECONDITION_H
#include "simplecondition.h"

class CompositeCondition
{
// required
public:
    enum Conector {EMPTY, AND, OR};
    typedef QList<QPair<Conector, Condition>> QConditionsList;

// members
public:
    QConditionsList conditions;
private:

// methods
public:
    CompositeCondition(const QString &input);
    bool Validate();
    void SetNameSpace(QMap<QString, Key>);
    QMap<QString, Key> getNameSpace();
    QString toString();
private:
    Conector parseConnector(const QString &conector);
};

#endif // COMPOSITECONDITION_H
