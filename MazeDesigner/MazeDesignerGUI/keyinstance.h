#ifndef KEYINSTANCE_H
#define KEYINSTANCE_H

#include <QPointF>
#include <QRectF>
#include "../Keys/key.h"
#include "../Keys/keyrepository.h"
#include "../Common/common.h"
#include "selectableinstance.h"

class KeyInstance : public QRectF, public SelectableInstance
{
public:
    /**
     * @brief KeyInstance constructor that creates a instance of a key from a given model and given space coordinates
     * the id if the instance is incrementaly auto-given
     * @param model the model of which the constructed object is instance of
     * @param x the x coordinate
     * @param y the y coordinate
     */
    KeyInstance(const Key & model, const qreal & x, const qreal & y);
    /**
     * @brief KeyInstance constructor that creates a instance of a key from a given model and given space coordinates
     * the id if the instance is incrementaly auto-given
     * @param model the model of which the constructed object is instance of
     * @param point the point in space where the key instance will be created
     */
    KeyInstance(const Key & model, const QPointF & point);
    ~KeyInstance() override {}
    QPolygonF boundPolygon() const override;
    QString instanceInfo() const override;
    /**
     * @brief fromJson creates a KeyInstance from a json object holding its information and the keyRepository
     * that holds all the key models. Necessary for obtaining the true Key model from its id.
     * Essentially deserializing the object
     * @param json the json to parse
     * @param repo the KeyRepository holding Key models
     * @return the newly created KeyInstance
     */
    static KeyInstance fromJson(const QJsonObject & json, const KeyRepository & repo);
    /**
     * @brief toJson creates a json object holding the necessary information to reconstruct this object.
     * Essentially serializing itself
     * @return the json object with the object info
     */
    QJsonObject toJson();
    /**
     * @brief getId returns the id of the object
     * @return the id of the object
     */
    int getId();
    const Key model;
private:
    /**
     * @brief KeyInstance private constructor to force giving a id and not do it automatically. see above
     * @param id
     * @param model
     * @param x
     * @param y
     */
    KeyInstance(int id, const Key & model, const qreal & x, const qreal & y);
    /**
     * @brief KeyInstance private constructor to force giving a id and not do it automatically. see above
     * @param id
     * @param model
     * @param point
     */
    KeyInstance(int id, const Key & model, const QPointF & point);

    static int ID;
    int id;
friend class InstanceCanvas;
};

#endif // KEYINSTANCE_H
