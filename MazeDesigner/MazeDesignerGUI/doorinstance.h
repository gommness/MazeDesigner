#ifndef DOORINSTANCE_H
#define DOORINSTANCE_H

#include <QLineF>
#include <QPolygonF>
#include <QJsonObject>
#include <QPainter>
#include "../Common/common.h"
#include "../ConditionParser/compositecondition.h"
#include "selectableinstance.h"

class DoorInstance : public QObject, public QLineF, public SelectableInstance
{
    Q_OBJECT
public:
    /**
     * @brief DoorInstance constructs a DoorInstance from a line. the id is incrementaly auto-given
     * @param line the line to perform the construction
     */
    DoorInstance(QLineF & line);
    DoorInstance(DoorInstance & other);
    ~DoorInstance() override;
    QPolygonF boundPolygon() const override;
    QString instanceInfo() const override;
    /**
     * @brief contains checks whether the given point is contained within what is considered the area of the door. That is,
     * if the point is close enough to the door
     * @param point the point to check
     * @return true if the point is close enough to the door to be considered "inside" the line
     */
    bool contains(const QPointF & point) const;
    /**
     * @brief fromJson creates a doorInstance object from a json. efectively deserializing the object
     * @param json the json to parse
     * @return the doorInstance represented by the json
     */
    DoorInstance(const QJsonObject & json);
    /**
     * @brief toJson creates a QJsonObject holding the information if the object
     * @return said JsonObject
     */
    QJsonObject toJson() const;

    QPair<QPointF, QPointF> separation() const;
    CompositeCondition getCondition1() const;
    CompositeCondition getCondition2() const;
    void drawSelf(QPainter & painter, QPointF offset) const;

public slots:
    void setConditions(CompositeCondition & cond1, CompositeCondition & cond2);

private:
    /**
     * @brief DoorInstance private constructor to force an id instead of auto incrementing
     * @param id the id to give to the object
     * @param line the line for the construction
     */
    DoorInstance(int id, QLineF & line);

    static QPolygonF lineArea(QLineF line);


    static int ID;
    int id;
    QPolygonF area;
    CompositeCondition *condition1;
    CompositeCondition *condition2;
friend class InstanceCanvas;
};

#endif // DOORINSTANCE_H
