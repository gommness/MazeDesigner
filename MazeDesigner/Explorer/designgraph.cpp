#include "designgraph.h"

DesignGraph::DesignGraph(InstanceCanvas &canvas)
{
    // build the QPainterPath to substract later
    QPainterPath path;
    for(auto door = canvas.doors.begin(); door != canvas.doors.end(); door++)
        path.addPolygon((*door)->boundPolygon());
    // remove the simplified path (just in case) of doors to be substracted
    QList<QPolygonF> shapes = canvas.design->shapes.subtracted(path.simplified()).simplified().toFillPolygons();
    // now shapes is the list of polygons that we'll use to create the DesignGraph!
    // we'll loop through all those polygons in order to create all RegionNodes
    for(auto poly = shapes.begin(); poly != shapes.end(); poly++){
        RegionNode * node = new RegionNode(&(*poly));
        nodes.append(node);
    }
    // so far we have all nodes of the design. We now have to set the transitions between them
    // for that, we'll iterate through the doors, get the points of separations and then iterate through
    // the nodes. when a node contains a point of the separation, we set it to be one of the nodes of the transition
    for(auto door = canvas.doors.begin(); door != canvas.doors.end(); door++){
        QPair<QPointF, QPointF> pointPair = (*door)->separation();
        Transition * tran = new Transition();

        TODO("SET THE CONDITION OF THE TRANSITION");

        for(auto node = nodes.begin(); node != nodes.end(); node++){

            // check if the node we are iterating through contains one or both of the points of the separation
            if((*node)->containsPoint(pointPair.first))
                tran->node1 = *node;
            if ((*node)->containsPoint(pointPair.second))
                tran->node2 = *node;

            if(tran->isValid()) // when both nodes have been found, it makes no sense to keep iterating. we break out of the loop
                break;
        }
        if(!tran->isValid()){ // if on the loop, for whatever reason we could not create a valid transition, free its memory
            delete tran;
        } else {
            // if the transition is well-formed, then we add it our list of transitions
            transitions.append(tran);
            tran->node1->transitions.append(tran); // and add the transition to the origin node's transition list
        }
    }
    // now we have all nodes and transitions between them. Now the only thing left is to give the nodes the items that are
    // contained in their polygons AND set the current node to the one that contains the startToken. Then init the empty inventory
    QList<KeyInstance*> keys; // we'll create an aux list so that we can subtract the already given items from it
                              // since an item cannot be contained in two different RegionNodes. Thus, when an item is inserted
                              // it makes no sense to consider said item in future iterarions
    keys.append(canvas.keys);
    current = nullptr;
    if(canvas.startToken == nullptr && nodes.length() > 0)
        current = nodes[0];
    for(auto node = nodes.begin(); node != nodes.end(); node++){
        // first, set the current node (starting point) to node being iterated if it contains the startToken
        if(current == nullptr && (*node)->containsPoint(canvas.startToken->center()))
            current = *node;
        // iterate through the unGiven keys to give them to the node they belong to
        for(int i = keys.length()-1; i >= 0; i--){
            if((*node)->containsPoint(keys[i]->center())){
                (*node)->items.append(keys[i]);
                keys.removeAt(i);
            }
        }

        // if the starting point is being set and there are no more keys to give, it makes no sense to keep iterating. break out
        if(current != nullptr && keys.length() <= 0)
           break;
    }

    inventory = new Inventory();
}

DesignGraph::DesignGraph(DesignGraph &other)
{
    this->nodes.append(other.nodes);
    this->transitions.append(other.transitions);
    this->inventory = new Inventory(*other.inventory);
    this->current = other.current;
}

DesignGraph::~DesignGraph()
{
    for(int i = nodes.length()-1; i >= 0; i--)
        delete nodes[i];
    delete inventory;
}
