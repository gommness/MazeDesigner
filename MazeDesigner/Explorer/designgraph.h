#ifndef DESIGNGRAPH_H
#define DESIGNGRAPH_H

#include "regionnode.h"
#include "inventory.h"
#include "../MazeDesignerGUI/instancecanvas.h"
#include "../Canvas/canvas.h"
#include <QPainterPath>
#include <QStack>
#include <QHash>
#include "../Common/common.h"

class DesignGraph
{
public:
    DesignGraph(InstanceCanvas & canvas);
    DesignGraph(DesignGraph & other);
    ~DesignGraph();

    /**
     * @brief expand
     * @return a list of all possible DesignGraphs that could resutl from any action from this DesignGraph
     */
    QList<DesignGraph*> expand();
    void simplify();
    void destroyNode(RegionNode * node);
    double heuristic() const;
    bool operator ==(const DesignGraph & other) const;
    RegionNode * getNode(const RegionNode & node) const;
    Transition * getTransition(const Transition & transition) const;
    int size() const;

    QList<RegionNode*> nodes;
    QList<Transition*> transitions;
    Inventory * inventory;
    RegionNode * current;

private:
    void fuse(QList<RegionNode*>);
    QList<QList<RegionNode *>> tarjanAlgorithm();
    QList<RegionNode*> stronglyConnect(RegionNode * node, QStack<RegionNode *> & stack, int & index);
    bool checkConnexion() const;
};


#endif // DESIGNGRAPH_H
