#include "condition.h"
#include <QRegExp>

Condition::Condition(QString condition)
{
    QRegExp exp("[ ]");
    QStringList list = condition.split(exp, QString::SkipEmptyParts);
    if(list.size() != 3)
        throw ErrorMalformedCondition("size of condition does not match");
    name = list[0];
    comparator = list[1];
    value = list[2];
}

Condition::Condition(QString string, QMap<QString, Key> *map) : Condition(string)
{
    setNameSpace(map);
}

bool Condition::validate()
{
    return nameSpace != nullptr && validateName() && validateValue() && validateComparator();
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

QString Condition::toString()
{
    return QString("Condition{"+name+" "+comparator+" "+value+"}");
}

bool Condition::validateName()
{
    return nameSpace->contains(name);
}

bool Condition::validateComparator()
{
    return COMPARATORS.contains(comparator);
}

bool Condition::validateValue()
{
    if((*nameSpace)[name].type == Key::Type::NUMERIC){
        bool ok;
        value.toDouble(&ok);
        return ok;
    } else if((*nameSpace)[name].type == Key::Type::BOOLEAN){
        QString val = value.toUpper();
        return val == "TRUE" || val == "FALSE";
    }
    return false;
}
