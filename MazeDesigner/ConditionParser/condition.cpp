#include "condition.h"

Condition::Condition(bool sat, KeyRepository * repo) : keyRepo(repo), satisfiable(sat) {}

Condition::Condition(const Condition &other)
{
    this->keyRepo = other.keyRepo;
    this->satisfiable = other.satisfiable;
}

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

Condition::CostList Condition::getCost() const
{
    return Condition::CostList();
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

const KeyRepository * Condition::getRepository()
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

