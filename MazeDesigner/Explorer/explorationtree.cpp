#include "explorationtree.h"
#include <algorithm>

QList<SelectableInstance *> ExplorationTree::explore(const InstanceCanvas &canvas)
{
    QList<SelectableInstance *> output;
    DesignGraph graph(canvas);
    QList<DesignGraph *> openNodes;
    QList<DesignGraph *> exploredNodes;
    openNodes.append(&graph);
    DesignGraph * current;
    if(!graph.isValid())
        return output;
    while(!openNodes.isEmpty()){
        current = openNodes.takeAt(0);
        exploredNodes.append(current);
        if(current->size() == 1){ // the "well designed" condition
            continue;
        }
        QList<DesignGraph *>  expansions = current->expand();
        if(expansions.isEmpty()){ // the "bad designed" condition
            output = current->instances;
            break;
        }
        removeRepeated(exploredNodes, expansions); // remove search states repeated
        openNodes.append(expansions); // the new nodes are nodes to be explored
        std::sort(openNodes.begin(), openNodes.begin(), ExplorationTree::heuristicLessThan); // sort the nodes by their heuristic
    }
    return output;
}

void ExplorationTree::removeRepeated(QList<DesignGraph *> &previous, QList<DesignGraph *> &expanded)
{
    for(int i = expanded.length()-1; i >= 0; i--){
        int pos = containsValue(previous, expanded[i]);
        if(pos != -1){
            expanded.removeAt(i);
            TODO("Maybe, delete the memory of the designGraph removed?");
        }
    }
}

bool ExplorationTree::heuristicLessThan(const DesignGraph *graph1, const DesignGraph *graph2)
{
    return graph1->heuristic() < graph2->heuristic();
}

int ExplorationTree::containsValue(QList<DesignGraph *> &list, DesignGraph *val)
{
    for(int i = 0; i < list.length(); i++)
        if(*list[i] == *val)
            return i;
    return -1;
}
