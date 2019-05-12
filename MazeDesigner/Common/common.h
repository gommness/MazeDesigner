#ifndef COMMON_H
#define COMMON_H

#include "common_global.h"
#include <QDebug>
#include <QPointF>
#define UMBRAL 0.1
#define POINTFORMAT "(%1 %2)"
#define POINTSPLITREGEX "[() ]"

#define TODO(x) printf("TODO (%s line %s) : %s", __FILE__, __LINE__, x)

namespace json {
    QPointF parsePoint(const QString & str);
};

class COMMONSHARED_EXPORT Common
{

public:
    Common();
};

#endif // COMMON_H
