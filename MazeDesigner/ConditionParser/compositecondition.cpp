#include "compositecondition.h"
#include <QJsonArray>

CompositeCondition::CompositeCondition(const QString &input, KeyRepository * repo)
{
    this->keyRepo = repo;
    QStringList list = CompositeCondition::customSplit(input);
    conditions = parseConditions(list, repo);
    // first condition in the QConditionsList will have the empty connector. the rest would have the corresponding connector
    // keep in mind that a Condition can be either a CompositeCondition or a SimpleCondition.
}

CompositeCondition::CompositeCondition(const QJsonObject &jObj, KeyRepository * repo)
{
    this->keyRepo = repo;

    if(jObj.contains("empty") && jObj["empty"].isBool()){
        if(jObj["empty"].toBool()){
            *this = CompositeCondition::emptyCondition();
            return;
        }
    } else {
        throw std::runtime_error("malformed condition");
    }

    if(jObj.contains("satisfiable") && jObj["satisfiable"].isBool()){
        this->satisfiable = jObj["satisfiable"].toBool();
    } else {
        this->satisfiable = true;
    }

    if(jObj.contains("condition") && jObj["condition"].isString()){
        QStringList cond = jObj["condition"].toString().split(" ");
        if(cond.length() < 2)
            throw std::runtime_error("malformed condition");
        *this = CompositeCondition(jObj["condition"].toString(), repo);
    } else {
        throw std::runtime_error("malformed condition");
    }
}

CompositeCondition::~CompositeCondition()
{
    for(int i = conditions.length(); i >= 0; i--){
        delete conditions[i].second;
    }
    conditions.clear();
}

bool CompositeCondition::validate() const
{
    Condition::validate();
    bool output = true;
    for(int i = 0; i < conditions.length(); i++)
        output &= conditions[i].second->validate();
    return output;
}

QString CompositeCondition::toString() const
{
    QString output = "";
    for(int i=0; i<conditions.length(); i++) {
        Connector aux = conditions[i].first;
        if(aux == Connector::OR)
            output += "OR";
        else if(aux == Connector::AND)
            output += "AND";
        output += " " + conditions[i].second->toString() + " ";
    }
    return output;
}

const CompositeCondition &CompositeCondition::emptyCondition()
{
    static const CompositeCondition output("");
    return output;
}

bool CompositeCondition::isSatisfiable() const
{
    if(isEmpty())
        return true;
    bool satisfiable = true;
    if(conditions.length() > 0)
        satisfiable = conditions[0].second->isSatisfiable();

    for(auto condition = conditions.begin(); condition != conditions.end(); condition++){
        if(condition->first == Connector::EMPTY)
            satisfiable = condition->second->isSatisfiable();
        else if(condition->first == Connector::OR)
            satisfiable |= condition->second->isSatisfiable();
        else
            satisfiable &= condition->second->isSatisfiable();
    }
    return satisfiable;
}

QJsonObject CompositeCondition::toJson() const
{
    QJsonObject output;
    output.insert("empty", isEmpty());
    output.insert("satisfiable", isSatisfiable());
    output.insert("condition", toString());
    return output;
}

bool CompositeCondition::isEmpty() const
{
    return *this == CompositeCondition::emptyCondition();
}

bool CompositeCondition::operator ==(const CompositeCondition &other) const
{
    return this->conditions == other.conditions;
}

CompositeCondition::CompositeCondition(CompositeCondition::QConditionsList &list, KeyRepository * repo)
{
    keyRepo = repo;
    conditions = list;
}

CompositeCondition::Connector CompositeCondition::parseConnector(const QString &connector)
{
    QString aux = connector.toUpper();
    if(aux == "OR" || aux == "||"){
        return Connector::OR;
    } else if(aux == "AND" || aux == "&&"){
        return Connector::AND;
    }
    return Connector::EMPTY;
}

CompositeCondition::QConditionsList CompositeCondition::parseConditions(const QStringList &list, KeyRepository * repo){
    int index = 0;
    return parseConditions(list, index, repo);
}

CompositeCondition::QConditionsList CompositeCondition::parseConditions(const QStringList &list, int &index, KeyRepository * repo){
    QConditionsList output; // list of Connector-Condition.
    Connector connector;
    Condition *condition;
    QPair<Connector, Condition*> pair = QPair<Connector, Condition*>();

    connector = Connector::EMPTY;

    for(; index < list.length(); index++){ // iterate throught the list of "words"
        if(list[index] == "("){ // if it is a condition between parenthesis, then it is composite.
            // thus we should create a composite condition from the sublist of conditions between paranthesis
            try{
                QConditionsList subCompositeConditionList = parseConditions(list, index);
                condition = new CompositeCondition(subCompositeConditionList, repo);
            } catch(ConditionError::Malformed &err){
                // if an exception is caught, we shall free all memory allocated within output and pass it through
                for(int j = output.length()-1; j >= 0; j--)
                    delete output[j].second;
                throw err;
            }
        } else if(list[index] == ")"){ // if it is the end of a composite condition
            return output; // we return the QConditionList that would make the CompositeCondition
                           // that was inside the paranthesis
        } else { // create SimpleCondition
            bool ok = false;
            QString conditionString = list[index];
            if(list[index] == "have"){
                index++;
                if(index >= list.length()){
                    // check that no Out Of Bounds happens
                    for(int j = output.length()-1; j >= 0; j--)
                        delete output[j].second;
                    throw ConditionError::Malformed("Unexpected end of condition");
                }
            }
            list[index].toInt(&ok);
            if(ok){ // if the simple condition had a number in it
                index++;
                if(index >= list.length()){
                    // check that no Out Of Bounds happens
                    for(int j = output.length()-1; j >= 0; j--)
                        delete output[j].second;
                    throw ConditionError::Malformed("Unexpected end of condition");
                }
                condition = new SimpleCondition(list[index-1], list[index], repo);
            } else { // if it was a numberless simple condition
                condition = new SimpleCondition("1", list[index], repo);
            }
        }
        // now, prepare the pair to be inserted.
        pair.first = connector;
        pair.second = condition;
        output.append(pair);
        // increment the index to check for either a connector or a end of parenthesis
        index++;
        if(index >= list.length()){
            // check that no Out Of Bounds happens
            for(int j = output.length()-1; j >= 0; j--)
                delete output[j].second;
            throw ConditionError::Malformed("Unexpected end of condition");
        }
        if(list[index] == ")") // if it is a ), then we'll exit the creation of this CompositeCondition
            return output;
        // if it was not a ")" the it has to be a connector. Otherwise we'll throw an exception.
        connector = parseConnector(list[index]);
        if (connector == Connector::EMPTY){
            delete condition;
            for(int j = output.length()-1; j >= 0; j--)
                delete output[j].second;
            throw ConditionError::Malformed("malformed condition");
        }
        // next index increment will happen within the loop body
    }
    return output;
}

/*
CompositeCondition::QConditionsList CompositeCondition::parseConditions(const QStringList &list){
    QConditionsList output; // list of Connector-Condition.
    Connector *connector;
    Condition *condition;
    QPair<Connector, Condition> pair = QPair<Connector, Condition>();

    connector = new Connector();
    *connector = Connector::EMPTY;
    condition = new Condition();

    for(int i = 0; i < list.length(); i++){ // iterate throught the list of "words"
        if(list[i] == "("){ // if it is a condition between parenthesis, then it is composite.
            // create a composite condition
        } else if(list[i] == ")"){ // if it is the end of a composite condition
            // we'd end the creation of this composite condition
            // then return it. BUT ALSO, we should somehow throw outside the
            // index of the QStringList that we are at right now !!!!
            // TODO do this last part
        } else {
            // create SimpleCondition
        }
        // TODO create the condition, either simple condition or composite condition
        //      then advance with the list until a connector is reached

        pair.first = *connector;
        pair.second = *condition;
        output.append(pair);

        *connector = parseConnector(list[i]);
    }
    delete connector;
    return output;
}
*/

QStringList CompositeCondition::customSplit(const QString &input)
{
    QStringList output;
    QStringList splittedList = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    foreach( QString word, splittedList){
        QStringList cleansedWord = word.split(QRegExp("\\b"));
        //cleansedWord.remove(QRegExp("[)(]")); // cleansed word would be the word but removing all "(" and ")" in it if they existed
        for(auto str = cleansedWord.begin(); str != cleansedWord.end(); str++){
            if(str->contains("()")) // if it contains the empty expresion, thus, the empty condition, we just won't include it
                continue;
            else if(str->contains("("))
                output.append("(");
            else if(str->contains(")"))
                output.append(")");
            else
                output.append(*str);
        }
    }
    return output;
}
