#ifndef DESIGNGRAPH_H
#define DESIGNGRAPH_H

#include "regionnode.h"
#include "inventory.h"
#include "../MazeDesignerGUI/instancecanvas.h"
#include "../Canvas/canvas.h"
#include <QPainterPath>
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
    double heuristic() const;
    bool operator ==(const DesignGraph & other) const;
    RegionNode * getNode(const RegionNode & node) const;

    QList<RegionNode*> nodes;
    QList<Transition*> transitions;
    Inventory * inventory;
    RegionNode * current;

private:
    bool checkConnexion() const;
};

#endif // DESIGNGRAPH_H
