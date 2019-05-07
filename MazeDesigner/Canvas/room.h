#ifndef ROOM_H
#define ROOM_H

#include <QRect>
#include <QString>
#include <QJsonObject>
//#define JSONROOMKEY "rooms"

class Room : public QRectF
{
public:
    Room(QRectF & rect, QString  name = "");

    QString getName() const;
    void setName(const QString &value);
    int32_t getId() const;
    QJsonObject toJson() const;
    void fromJson(const QJsonObject & json);

private:
    static int16_t ID;
    QString name;
    int32_t id;
};

#endif // ROOM_H
