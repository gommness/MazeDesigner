#ifndef REGION_NODE_H
#define REGION_NODE_H

#include <QList>
#include <QRegion>
#include "../MazeDesignerGUI/keyinstance.h"
#include "../ConditionParser/simplecondition.h"
#include "../Common/common.h"
class RegionNode;

class Transition
{
public:
    Transition(RegionNode * node1 = nullptr, RegionNode * node2 = nullptr, SimpleCondition * cond = nullptr);
    ~Transition();

    bool isValid() const;
    bool operator ==(const Transition & other) const;
    bool operator !=(const Transition & other) const;

    RegionNode * node1 = nullptr;
    RegionNode * node2 = nullptr;
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
    static RegionNode * fusion(RegionNode &n1, RegionNode &n2);
    bool containsPoint(const QPointF & point) const;
private:
    static uint qHash(const RegionNode &key);
    bool checkConnexion(QHash<RegionNode*, QList<RegionNode*>> & map);

    QPolygonF *parent;
};


#endif
