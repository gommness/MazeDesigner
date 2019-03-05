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
    QMap<QString, Key> *nameSpace = nullptr;

/// methods
public:
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

namespace  ConditionError {
    class Malformed : public std::runtime_error {
    public:
        Malformed(const std::string& msg = "") : runtime_error(msg) {}
    };

    class SyntaxError : public Malformed{
        public:
        SyntaxError(const std::string& msg = "") : Malformed("Syntax Error: " + msg) {}
    };

    class MissingNameSpace : public Malformed{
        public:
        MissingNameSpace(const std::string& msg = "") : Malformed(msg) {}
    };

    class UnknownIdentifier : public Malformed{
        public:
        UnknownIdentifier(const std::string& msg = "") : Malformed(msg) {}
    };
}

#endif // CONDITION_H
