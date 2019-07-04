#include "simplecondition.h"
#include <QRegExp>

// syntax: [have] [<number>] <item>

SimpleCondition::SimpleCondition(QString number, QString key, KeyRepository* repo, bool sat)
    : Condition(sat, repo), name(key), value(number.toUInt()) {}

SimpleCondition::SimpleCondition(QString condition, KeyRepository* repo, bool sat)
{
    satisfiable = sat;
    keyRepo = repo;
    name = "";
    value = 0;

    QRegExp exp("[ ]");
    QStringList list = condition.split(exp, QString::SkipEmptyParts);

    int size = list.size();
    if(size > 3) // if the condition inserted does not match syntax
        throw ConditionError::SyntaxError("condition <" +condition.toStdString()+"> does not match syntax [have] [<number>] item");

    if(size >= 1)
        name = list.last(); // the name of the required key is allways last in a simple condition
    else
        name = ""; // the empty name means the empty condition, which is allways true

    bool ok = false;
    if(size >= 2)
        value = list[size-2].toUInt(&ok); // not the last word, but the word before the last
        if(!ok)
            value = 0;
    else
        value = 1; // value is not required in the syntax. if omitted, then the condition assumes it means have 1 <item>
}

SimpleCondition::SimpleCondition(const QJsonObject &jObj, KeyRepository * repo)
{
    this->keyRepo = repo;

    if(jObj.contains("satisfiable") && jObj["satisfiable"].isBool()){
        this->satisfiable = jObj["satisfiable"].toBool();
    } else {
        this->satisfiable = true;
    }

    if(jObj.contains("empty") && jObj["empty"].isBool()){
        if(jObj["empty"].toBool()){
            *this = SimpleCondition::emptyCondition();
            return;
        }
    } else {
        throw std::runtime_error("malformed condition");
    }

    if(jObj.contains("condition") && jObj["condition"].isString()){
        QStringList cond = jObj["condition"].toString().split(" ");
        if(cond.length() < 2)
            throw std::runtime_error("malformed condition");
        value = cond[0].toUInt();
        name = cond[1];
    } else {
        throw std::runtime_error("malformed condition");
    }
}

SimpleCondition::SimpleCondition(const SimpleCondition &other) : Condition(other)
{
    this->name = other.name;
    this->value = other.value;
}

bool SimpleCondition::validate() const
{
    if(isEmpty())
        return true;

    Condition::validate();
    bool output = validateName();
    if(!output)
        throw ConditionError::UnknownIdentifier(name.toStdString());
    return output;
}

Condition::CostList SimpleCondition::getCost() const
{
    Condition::CostList output;
    Key key = keyRepo->keyWithName(name);
    if(key.isValid())
        output.append(Cost(&key, value));
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
        return QString(QString::number(value) +" " + name);
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

