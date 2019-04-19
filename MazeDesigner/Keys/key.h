#ifndef KEY_H
#define KEY_H

#include <QString>

class Key
{
public:
    Key(const QString & name = "", const bool & value = false);
    QString getName() const;
    void setName(const QString & name);
    bool isPowerUp() const;
    void setPowerUp(const bool & value);
    QString toString();
private:
    static int ID;
    QString name;
    bool powerUp;
};

#endif // KEY_H
