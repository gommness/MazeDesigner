#ifndef KEYREPOSITORY_H
#define KEYREPOSITORY_H

#include <QMap>
#include "key.h"

class KeyRepository : public QList<Key>
{

public:
};

KeyRepository& operator<<(KeyRepository & repo, Key const & val);

#endif // KEYREPOSITORY_H
