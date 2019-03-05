#include "compositecondition.h"

CompositeCondition::CompositeCondition(const QString &input)
{
    QStringList list = CompositeCondition::customSplit(input);
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

CompositeCondition::Conector CompositeCondition::parseConnector(const QString &conector)
{

}

QStringList CompositeCondition::customSplit(const QString &input)
{
    QStringList output;
    QStringList splittedList = input.split(QRegExp("[ ]"), QString::SkipEmptyParts);
    foreach( QString word, splittedList){
        QString cleansedWord(word);
        cleansedWord.remove(QRegExp("[())]")); // cleansed word would be the word but removing all "(" and ")" in it if they existed

        if(word.front() == "(") // if the word started with (, add it as a string to the output
            output.append("(");

        output.append(cleansedWord); // add the cleansed word

        if(word.back() == ")") // if the word ended with ), add it as a string to the output
            output.append(")");
    }
    return output;
}
