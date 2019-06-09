#include "condition.h"

Condition::Condition(bool sat, KeyRepository * repo) : keyRepo(repo), satisfiable(sat) {}

bool Condition::validate() const
{
    if(!keyRepo)
        throw ConditionError::MissingNameSpace();
    return true;
}

bool Condition::isSatisfiable() const
{
    return satisfiable;
}

const Condition &Condition::emptyCondition()
{
    const static Condition output;
    return output;
}

const Condition &Condition::unsatCondition()
{
    static const Condition output(false);
    return output;
}

bool Condition::isEmpty() const
{
    return *this == Condition::emptyCondition();
}

void Condition::setKeyRepository(KeyRepository *map)
{
    keyRepo = map;
}

KeyRepository * Condition::getNameSpace()
{
    return keyRepo;
}

QString Condition::toString() const
{
    return QString("{Condition EMPTY}");
}

bool Condition::operator ==(const Condition &other) const
{
    return this->satisfiable == other.satisfiable;
}

QJsonObject Condition::toJson() const
{
    return QJsonObject();
}

