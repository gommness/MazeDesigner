#include "simplecondition.h"
#include <QRegExp>

// syntax: [have] [<number>] <item>

SimpleCondition::SimpleCondition(QString number, QString key)
{
    name = key;
    value = number;
}

SimpleCondition::SimpleCondition(QString condition)
{
    QRegExp exp("[ ]");
    QStringList list = condition.split(exp, QString::SkipEmptyParts);

    int size = list.size();
    if(size > 3) // if the condition inserted does not match syntax
        throw ConditionError::SyntaxError("condition <" +condition.toStdString()+"> does not match syntax [have] [<number>] item");

    if(size >= 1)
        name = list.last(); // the name of the required key is allways last in a simple condition
    else
        name = ""; // the empty name means the empty condition, which is allways true

    if(size >= 2)
        value = list[size-2]; // not the last word, but the word before the last
    else
        value = "1"; // value is not required in the syntax. if omitted, then the condition assumes it means have 1 <item>
}

SimpleCondition::SimpleCondition(QString string, QMap<QString, Key> *map) : SimpleCondition(string)
{
    setNameSpace(map);
}

bool SimpleCondition::validate()
{
    Condition::validate();
    bool output = validateName();
    if(!output)
        throw ConditionError::UnknownIdentifier(name.toStdString());
    output &= validateValue();
    if(!output)
        throw ConditionError::Malformed("value \""+value.toStdString()+"\" is not numeric");
    return output;
}

QString SimpleCondition::toString()
{
    if(name != "")
        return QString("SimpleCondition{ have " + value + name + "}");
    else
        return QString("SimpleCondition{}");
}





bool SimpleCondition::validateName()
{
    return nameSpace->contains(name) || name == ""; // the name either exists in the nameSpace or this is the empty condition
}

bool SimpleCondition::validateValue()
{
    bool ok;
    value.toDouble(&ok);
    return ok; // true if the value can be converted to a number
}
