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
    SimpleCondition(QString number, QString key);
    SimpleCondition(QString string);
    SimpleCondition(QString string, QMap<QString, Key> *map);
    bool validate();
    void setNameSpace(QMap<QString, Key> *map);
    QMap<QString, Key> getNameSpace();
    QString toString();

private:
    bool validateName();
    bool validateValue();
};

#endif // CONDITION_H
