#ifndef REGION_NODE_H
#define REGION_NODE_H

#include <QList>
#include <QRegion>
#include "../MazeDesignerGUI/keyinstance.h"
#include "../ConditionParser/condition.h"
#include "../Common/common.h"
class RegionNode;

class Transition
{
public:
    Transition(RegionNode * node1 = nullptr, RegionNode * node2 = nullptr, Condition * cond = nullptr);

    bool isValid() const;

    RegionNode * node1 = nullptr;
    RegionNode * node2 = nullptr;
    Condition * condition = nullptr;
};

class RegionNode
{
public:
    QList<KeyInstance*> items;
    QList<Transition*> transitions;


    RegionNode(QPolygonF * parent);
    ~RegionNode();
    bool operator ==(const RegionNode &other);
    static RegionNode * fusion(RegionNode &n1, RegionNode &n2);
    bool containsPoint(const QPointF & point) const;
private:
    static uint qHash(const RegionNode &key);
    bool checkConnexion(QHash<RegionNode*, QList<RegionNode*>> & map);

    QPolygonF *parent;
};


#endif
