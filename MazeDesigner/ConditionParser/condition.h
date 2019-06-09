#ifndef CONDITION_H
#define CONDITION_H

#include <QString>
#include <QMap>
#include "../Keys/key.h"
#include "../Keys/keyrepository.h"

class Condition
{
public:
    KeyRepository *keyRepo = nullptr;

    Condition(bool sat = true, KeyRepository * repo = nullptr);
    virtual ~Condition() {}
    virtual bool validate() const;
    virtual bool isSatisfiable() const;
    static const Condition & emptyCondition();
    static const Condition & unsatCondition();
    virtual bool isEmpty() const;
    virtual void setKeyRepository(KeyRepository *map);
    virtual KeyRepository * getNameSpace();
    virtual QString toString() const;
    virtual bool operator ==(const Condition & other) const;
    virtual QJsonObject toJson() const;
protected:
    bool satisfiable = true;
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
