#ifndef SIMPLECONDITION_H
#define SIMPLECONDITION_H

#include "condition.h"

class SimpleCondition : public Condition
{
/// members
public:
    QString name;
    // QString comparator;
    QString value;
private:

/// methods
public:
    SimpleCondition(QString number, QString key, KeyRepository * repo = nullptr, bool sat = true);
    SimpleCondition(QString string, KeyRepository * repo = nullptr, bool sat = true);
    SimpleCondition(const QJsonObject & jObj, KeyRepository * repo = nullptr);
    SimpleCondition(const SimpleCondition & other);
    bool validate() const override;
    bool operator ==(const SimpleCondition & cond) const;
    static const SimpleCondition & emptyCondition();
    static const SimpleCondition & unsatCondition();
    bool isEmpty() const override;
    //void setNameSpace(QMap<QString, Key> *map);
    //QMap<QString, Key> getNameSpace();
    QString toString() const override;
    QJsonObject toJson() const override;
private:
    bool validateName() const;
    bool validateValue() const;
};

#endif // CONDITION_H
