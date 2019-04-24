#ifndef KEYREPOSITORY_H
#define KEYREPOSITORY_H

#include <QMap>
#include <QObject>
#include "key.h"

class KeyRepository : public QList<Key>, public QObject
{
public:
    KeyRepository();
    KeyRepository& operator=(const KeyRepository & other);
    QString toString();
public slots:
    //void onKeyChanged(const int & index, const QString & keyname, const bool & value);
};

KeyRepository& operator<<(KeyRepository & repo, Key const & val);

#endif // KEYREPOSITORY_H