#ifndef ROOM_H
#define ROOM_H

#include <QRect>
#include <QString>
#include <QJsonObject>


class Room : public QRectF
{
public:
    /**
     * @brief Room creates a room from a given rectangle and a given name. The id is incrementaly auto-given
     * @param rect the rectangle that is the area of the room
     * @param name the name of the room. if empty, it will generate a auto-name based on id
     */
    Room(QRectF & rect, QString  name = "");
    /**
     * @brief Room creates a room from a json object that holds the information
     * @param json the json object that represents the room
     */
    Room(const QJsonObject & json);
    /**
     * @brief getName retrieves the name of the room
     * @return the name of the room
     */
    QString getName() const;
    /**
     * @brief setName sets the name of the room
     * @param value the name to be set
     */
    void setName(const QString &value);
    /**
     * @brief getId retrieves the id of the room
     * @return the id of the room
     */
    int32_t getId() const;
    /**
     * @brief toJson creates a json object containing its information so that this room can be reconstructed
     * @return the json object representation of the room
     */
    QJsonObject toJson() const;
    //void fromJson();

private:
    static int16_t ID;
    QString name;
    int32_t id;
};

#endif // ROOM_H
