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
    CompositeCondition(const QString &input, KeyRepository * repo = nullptr, bool sat = true);
    CompositeCondition(const QJsonObject & jobj, KeyRepository * repo = nullptr);
    ~CompositeCondition() override;
    bool validate() const override;
    QString toString() const override;
    static const CompositeCondition & emptyCondition();
    bool isEmpty() const override;
    bool isSatisfiable() const override;
    QJsonObject toJson() const override;
    bool operator ==(const CompositeCondition & other) const;
private:
    CompositeCondition(QConditionsList &list, KeyRepository * repo = nullptr);
    static Connector parseConnector(const QString &conector);
    static QConditionsList parseConditions(const QStringList &list, KeyRepository * repo = nullptr);
    static QConditionsList parseConditions(const QStringList &list, int &index, KeyRepository * repo = nullptr);
    static QStringList customSplit(const QString &input);
};

#endif // COMPOSITECONDITION_H
