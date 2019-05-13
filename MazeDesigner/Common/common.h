#ifndef COMMON_H
#define COMMON_H

#include "common_global.h"
#include <QDebug>
#include <QPointF>
#define UMBRAL 0.1
#define POINTFORMAT "(%1 %2)"
#define POINTSPLITREGEX "[() ]"

#define TODO(x) printf("TODO (%s line %d) : %s\n", __FILE__, __LINE__, x)

namespace json {
    /**
     * @brief parsePoint parses a string into a point
     * @param str jsonValue in string form to be parsed
     * @return the point represented in the string
     */
    QPointF parsePoint(const QString & str);
};

class COMMONSHARED_EXPORT Common
{

public:
    Common();
};

#endif // COMMON_H
