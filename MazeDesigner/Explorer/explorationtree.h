#ifndef EXPLORATIONTREE_H
#define EXPLORATIONTREE_H

#include "../MazeDesignerGUI/instancecanvas.h"
#include "../MazeDesignerGUI/selectableinstance.h"
#include "designgraph.h"

class ExplorationTree
{
public:
    //ExplorationTree();
    static QList<SelectableInstance *> explore(const InstanceCanvas & );
private:
    static void removeRepeated(QList<DesignGraph*> &previous, QList<DesignGraph*> &expanded);
    static bool heuristicLessThan(const DesignGraph * graph1, const DesignGraph * graph2);
    static int containsValue(QList<DesignGraph*> &previous, DesignGraph* val);
};

#endif // EXPLORATIONTREE_H
