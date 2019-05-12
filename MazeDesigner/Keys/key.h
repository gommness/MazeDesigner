#ifndef KEY_H
#define KEY_H

#include <QString>
#include <QJsonObject>

class Key
{
public:
    Key(const QString & name = "", const bool & value = false);
    Key(const QJsonObject & json);
    static Key & invalidKey();
    QString getName() const;
    bool isValid() const;
    void setName(const QString & name);
    bool isPowerUp() const;
    void setPowerUp(const bool & value);
    QString toString();
    QJsonObject toJson();
private:
    Key(int id);
    static int ID;
    int id;
    QString name;
    bool powerUp;

friend class KeyRepository;
};

#endif // KEY_H
