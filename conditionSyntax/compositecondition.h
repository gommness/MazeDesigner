#ifndef COMPOSITECONDITION_H
#define COMPOSITECONDITION_H
#include "simplecondition.h"

class CompositeCondition : Condition
{
// required
public:
    enum Connector {EMPTY, AND, OR};
    typedef QList<QPair<Connector, Condition*>> QConditionsList;

// members
public:
private:
    QConditionsList conditions;

// methods
public:
    CompositeCondition(const QString &input);
    bool Validate();
    QString toString();
private:
    CompositeCondition(QConditionsList &list);
    static Connector parseConnector(const QString &conector);
    static QConditionsList parseConditions(const QStringList &list);
    static QConditionsList parseConditions(const QStringList &list, int &index);
    static QStringList customSplit(const QString &input);
};

#endif // COMPOSITECONDITION_H
