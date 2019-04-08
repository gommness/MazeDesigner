#ifndef CONDITION_H
#define CONDITION_H

#include <QString>
#include <QMap>
#include "key.h"

class Condition
{
public:
    QMap<QString, Key> *nameSpace = nullptr;

    Condition();
    virtual ~Condition();
    virtual bool validate();
    virtual void setNameSpace(QMap<QString, Key> *map);
    virtual QMap<QString, Key> getNameSpace();
    virtual QString toString();
private:
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
