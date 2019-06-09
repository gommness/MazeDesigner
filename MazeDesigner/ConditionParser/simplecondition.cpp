#include "simplecondition.h"
#include <QRegExp>

// syntax: [have] [<number>] <item>

SimpleCondition::SimpleCondition(QString number, QString key, KeyRepository* repo, bool sat)
    : Condition(sat, repo), name(key), value(number) {}

SimpleCondition::SimpleCondition(QString condition, KeyRepository* repo, bool sat)
{
    satisfiable = sat;
    keyRepo = repo;
    name = "";
    value = "";

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

SimpleCondition::SimpleCondition(const QJsonObject &jObj, KeyRepository * repo)
{
    if(jObj.contains("satisfiable") && jObj["satisfiable"].isBool()){
        this->satisfiable = jObj["satisfiable"].toBool();
    } else {
        this->satisfiable = true;
    }

    if(jObj.contains("condition") && jObj["condition"].isString()){
        QStringList cond = jObj["condition"].toString().split(" ");
        if(cond.length() < 2)
            throw std::runtime_error("malformed condition");
        value = cond[0];
        name = cond[1];
    } else {
        throw std::runtime_error("malformed condition");
    }

    if(jObj.contains("empty") && jObj["empty"].isBool()){
        if(jObj["empty"].toBool())
            *this = SimpleCondition::emptyCondition();
    } else {
        throw std::runtime_error("malformed condition");
    }

    this->keyRepo = repo;
}

bool SimpleCondition::validate() const
{
    if(isEmpty())
        return true;

    Condition::validate();
    bool output = validateName();
    if(!output)
        throw ConditionError::UnknownIdentifier(name.toStdString());
    output &= validateValue();
    if(!output)
        throw ConditionError::Malformed("value \""+value.toStdString()+"\" is not numeric");
    return output;
}

bool SimpleCondition::operator ==(const SimpleCondition &cond) const
{
    return this->satisfiable == cond.satisfiable && this->name == cond.name && this->value == cond.value;
}

const SimpleCondition &SimpleCondition::emptyCondition()
{
    static const SimpleCondition output("");
    return output;
}

const SimpleCondition &SimpleCondition::unsatCondition()
{
    static const SimpleCondition output("", nullptr, false);
    return output;
}

bool SimpleCondition::isEmpty() const
{
    return *this == SimpleCondition::emptyCondition();
}

QString SimpleCondition::toString() const
{
    if(!isEmpty())
        return QString(value +" " + name);
    else
        return QString("");
}

QJsonObject SimpleCondition::toJson() const
{
    QJsonObject output;
    output.insert("empty", isEmpty());
    output.insert("satisfiable", satisfiable);
    output.insert("condition", toString());
    return output;
}

bool SimpleCondition::validateName() const
{
    return keyRepo->contains(name); // the name either exists in the nameSpace or this is the empty condition
}

bool SimpleCondition::validateValue() const
{
    if(value.isEmpty())
        return true;
    bool ok;
    value.toDouble(&ok);
    return ok; // true if the value can be converted to a number
}
