#include "compositecondition.h"

CompositeCondition::CompositeCondition(const QString &input)
{
    QStringList list = CompositeCondition::customSplit(input);
    conditions = parseConditions(list);
    // first condition in the QConditionsList will have the empty connector. the rest would have the corresponding connector
    // keep in mind that a Condition can be either a CompositeCondition or a SimpleCondition.
}

bool CompositeCondition::Validate()
{
    bool output = true;
    for(int i = 0; i < conditions.length(); i++)
        output &= conditions[i].second->validate();
    return output;
}

QString CompositeCondition::toString()
{
    QString output = "CompositeCondition{";
    for(int i=0; i<conditions.length(); i++) {
        Connector aux = conditions[i].first;
        if(aux == Connector::OR)
            output += "OR";
        else if(aux == Connector::AND)
            output += "AND";
        output += " " + conditions[i].second->toString() + " ";
    }
    return output + "}";
}

CompositeCondition::CompositeCondition(CompositeCondition::QConditionsList &list)
{
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

CompositeCondition::QConditionsList CompositeCondition::parseConditions(const QStringList &list){
    int index = 0;
    return parseConditions(list, index);
}

CompositeCondition::QConditionsList CompositeCondition::parseConditions(const QStringList &list, int &index){
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
                condition = new CompositeCondition(subCompositeConditionList);
            } catch(ConditionError::Malformed &err){
                for(int j = output.length()-1; j >= 0; j--)
                    delete output[j].second;
                throw err;
            }
        } else if(list[index] == ")"){ // if it is the end of a composite condition
            return output; // we return the QConditionList that would make the CompositeCondition
                           // that was inside the paranthesis
        } else {
            // create SimpleCondition
            bool ok = false;
            QString conditionString = list[index];
            if(list[index] == "have")
                index++;
            list[index].toInt(&ok);
            if(ok){
                index++;
                condition = new SimpleCondition(list[index-1], list[index]);
            } else {
                condition = new SimpleCondition("1", list[index]);
            }
        }

        pair.first = connector;
        pair.second = condition;
        output.append(pair);

        connector = parseConnector(list[index]);
        if (connector == Connector::EMPTY){
            delete condition;
            for(int j = output.length()-1; j >= 0; j--)
                delete output[j].second;
            throw ConditionError::Malformed("malformed condition");
        }
    }
    return output;
}

QStringList CompositeCondition::customSplit(const QString &input)
{
    QStringList output;
    QStringList splittedList = input.split(QRegExp("[ ]"), QString::SkipEmptyParts);
    foreach( QString word, splittedList){
        QString cleansedWord(word);
        cleansedWord.remove(QRegExp("[)()]")); // cleansed word would be the word but removing all "(" and ")" in it if they existed

        if(word.front() == "(") // if the word started with (, add it as a string to the output
            output.append("(");

        output.append(cleansedWord); // add the cleansed word

        if(word.back() == ")") // if the word ended with ), add it as a string to the output
            output.append(")");
    }
    return output;
}
