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
            tran->condition = new SimpleCondition((*door)->getCondition1()); // finally, set the transition condition to a NEW cond
            // is important that the transition contains a new condition because the condition will be exploration-dependent modified
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
    // we have to iterate through all the nodes that other has
    for(auto node = other.nodes.begin(); node != other.nodes.end(); node++){
        //check if we have already collected this node (can happen bc something later)
        RegionNode * nodeOrigin = this->getNode(**node);
        if(nodeOrigin == nullptr){ // if it was not collected, create and collect it
            nodeOrigin = new RegionNode(**node);
            this->nodes.append(nodeOrigin);
        }
        // now we iterate through all the node's transitions
        for(auto tran = (*node)->transitions.begin(); tran != (*node)->transitions.end(); tran++){
            // check if a neighbour through a given transition has been collected
            RegionNode * nodeDest = this->getNode(*(*tran)->node2);
            if(nodeDest == nullptr){ // if it hasn't, create and collect it
                nodeDest = new RegionNode(*(*tran)->node2);
                this->nodes.append(nodeDest);
            }
            // then, whether it has been previously collected ot no, we create and collect their transition
            // keep in mind that a node might have multiple transitions to the same node, and we have to copy those too!
            // so even though we may have collected a node, the transition we are iterating through must be copied
            Transition * tr = new Transition(nodeOrigin, nodeDest, new SimpleCondition(*(*tran)->condition));
            nodeOrigin->transitions.append(tr);
            this->transitions.append(tr);
        }

    }
    // this will not create a copy of the items, only will append their references
    this->inventory = other.inventory;
    // current node will be the corresponding copied node of other's current node
    this->current = this->getNode(*other.current);
}

DesignGraph::~DesignGraph()
{
    for(int i = nodes.length()-1; i >= 0; i--)
        delete nodes[i];
    delete inventory;
}

QList<DesignGraph *> DesignGraph::expand()
{
    QList<DesignGraph *> output;
    // first: all the transitions from current node to another node
    for(auto tran = current->transitions.begin(); tran != current->transitions.end(); tran++){
        TODO("obtener coste de transicion (e.d. de la condicion). Si se puede asumir el coste, realizar la expansion");
        TODO("al realizar estas expansiones, recordar cambiar las condiciones de las transiciones para que queden abiertas");
    }
    // second: all the collection of items
    for(int i = 0; i < current->items.length(); i++){
        //create a new search state
        DesignGraph * graph = new DesignGraph(*this);
        //add the item to the inventory of the new search state
        graph->inventory->collect(current->items[i]);
        //remove the item from the new search state's current node
        graph->current->items.removeAt(i);
        output.append(graph);
    }
    return output;
}

bool DesignGraph::operator ==(const DesignGraph &other) const
{
    if(transitions.length() != other.transitions.length() || nodes.length() != other.nodes.length())
        return false;
    for (int i=0; i<transitions.length(); i++) {
        if(*transitions[i] != *other.transitions[i])
            return false;
    }
    for(int i=0; i<nodes.length(); i++){
        if(*nodes[i] != *other.nodes[i])
            return false;
    }
    if(*inventory != *other.inventory)
        return false;
    return (*current != *other.current);
}

RegionNode *DesignGraph::getNode(const RegionNode &node) const
{
    for(auto n = nodes.begin(); n != nodes.end(); n++)
        if(**n == node)
            return *n;
    return nullptr;
}
