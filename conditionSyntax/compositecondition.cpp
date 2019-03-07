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

}

void CompositeCondition::SetNameSpace(QMap<QString, Key>)
{

}

QMap<QString, Key> CompositeCondition::getNameSpace()
{

}

QString CompositeCondition::toString()
{

}

CompositeCondition::Connector CompositeCondition::parseConnector(const QString &conector)
{

}

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
