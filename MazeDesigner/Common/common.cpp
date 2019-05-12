#include "common.h"
#include <QStringList>

Common::Common()
{
}

QPointF json::parsePoint(const QString &str)
{
    QRegExp regex(POINTSPLITREGEX);
    QStringList splitted = str.split(regex, QString::SplitBehavior::SkipEmptyParts);
    if(splitted.size() < 2) // if there is less than 2 doubles, it ain't no point pal!
        throw std::runtime_error("error while parsing point: "+str.toUtf8()); // we'll panic
    return QPointF(splitted[0].toDouble(), splitted[1].toDouble());
}
