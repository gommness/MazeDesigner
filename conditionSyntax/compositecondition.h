#ifndef COMPOSITECONDITION_H
#define COMPOSITECONDITION_H
#include "simplecondition.h"

class CompositeCondition
{
// required
public:
    enum Connector {EMPTY, AND, OR};
    typedef QList<QPair<Connector, Condition>> QConditionsList;

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
    Connector parseConnector(const QString &conector);
    QConditionsList parseConditions(const QStringList &list);
    QStringList customSplit(const QString &input);
};

#endif // COMPOSITECONDITION_H
