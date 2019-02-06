#ifndef CONDITION_H
#define CONDITION_H

#include <QString>
#include <QMap>
#include "key.h"

static const QStringList COMPARATORS{"==", "!=", "<", ">", "<=", ">="};

class Condition
{
/// members
public:
    QString name;
    QString comparator;
    QString value;
private:
    QMap<QString, Key> *nameSpace = nullptr;

/// methods
public:
    Condition(QString string);
    Condition(QString string, QMap<QString, Key> *map);
    bool validate();
    void setNameSpace(QMap<QString, Key> *map);
    QMap<QString, Key> getNameSpace();
    QString toString();

private:
    bool validateName();
    bool validateComparator();
    bool validateValue();
};

class ErrorMalformedCondition : public std::runtime_error {
public: ErrorMalformedCondition(const std::string& msg = "") : runtime_error(msg) {}
};

#endif // CONDITION_H
