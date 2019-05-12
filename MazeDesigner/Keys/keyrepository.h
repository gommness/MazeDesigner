#ifndef KEYREPOSITORY_H
#define KEYREPOSITORY_H

#include <QMap>
#include <QObject>
#include "key.h"

#define JSONKEYKEY "keys"
#define JSONKEYMAXID "MAX-ID"
#define JSONKEYREPOKEY "key-repository"

class KeyRepository : public QList<Key>, public QObject
{
public:
    KeyRepository();
    KeyRepository& operator=(const KeyRepository & other);
    QString toString();
    bool contains(const QString & keyname) const;
    bool contains(int ID) const;
    const Key & keyWithId(int ID) const;
    void writeJson(QJsonObject & json);
    void readJson(const QJsonObject & json);
public slots:
    //void onKeyChanged(const int & index, const QString & keyname, const bool & value);
};

KeyRepository& operator<<(KeyRepository & repo, Key const & val);

#endif // KEYREPOSITORY_H
