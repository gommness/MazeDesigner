#ifndef REGION_NODE_H
#define REGION_NODE_H

#include <QList>
#include <QRegion>
#include "../MazeDesignerGUI/keyinstance.h"
#include "../MazeDesignerGUI/doorinstance.h"
#include "../ConditionParser/simplecondition.h"
#include "../Common/common.h"
class RegionNode;

class Transition
{
public:
    Transition(RegionNode * node1 = nullptr, RegionNode * node2 = nullptr,
               DoorInstance * door = nullptr, SimpleCondition * cond = nullptr);
    ~Transition();

    bool isValid() const;
    bool isOpen() const;
    Condition::CostList getCost() const;
    bool operator ==(const Transition & other) const;
    bool operator !=(const Transition & other) const;

    RegionNode * node1 = nullptr;
    RegionNode * node2 = nullptr;
    DoorInstance * door = nullptr;
    SimpleCondition * condition = nullptr;
};

class RegionNode
{
public:
    QList<KeyInstance*> items;
    QList<Transition*> transitions;


    RegionNode(QPolygonF * parent);
    RegionNode(RegionNode & other);
    ~RegionNode();
    bool operator ==(const RegionNode &other) const;
    bool operator !=(const RegionNode &other) const;
    static RegionNode * fusion(QList<RegionNode *> & nodes);
    bool containsPoint(const QPointF & point) const;
    QList<Transition *> getOpenTransitions() const;
private:
    static uint qHash(const RegionNode &key);
    bool checkConnexion(QHash<RegionNode*, QList<RegionNode*>> & map);

    QSet<QPolygonF*> parents;
    // the next attributes are required for the Tarjan's strongly connected components algorithm
    int index = -1;
    int lowLink = -1;
    bool onStack = false;

    friend class DesignGraph;
};


#endif
