#include "condition.h"

Condition::Condition(){
    nameSpace = new QMap<QString, Key>();
}

bool Condition::validate()
{
    if(!nameSpace)
        throw ConditionError::MissingNameSpace();
    return true;
}

void Condition::setNameSpace(QMap<QString, Key> *map)
{
    nameSpace = map;
}

QMap<QString, Key> Condition::getNameSpace()
{
    if(nameSpace == nullptr)
        return QMap<QString, Key>();
    else
        return QMap<QString, Key>(*nameSpace);
}

