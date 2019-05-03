#ifndef ROOM_H
#define ROOM_H

#include <QRect>
#include <QString>

class Room : public QRectF
{
public:
    Room(QRectF & rect, QString  name = "");

    QString getName() const;
    void setName(const QString &value);
    int16_t getId() const;

private:
    static int16_t ID;
    QString name;
    int16_t id;
};

#endif // ROOM_H
