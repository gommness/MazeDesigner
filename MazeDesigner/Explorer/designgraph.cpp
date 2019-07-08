#include "designgraph.h"
#include <QtMath>

DesignGraph::DesignGraph(const InstanceCanvas &canvas)
{
    // build the QPainterPath to substract later
    QPainterPath path;
    for(auto door = canvas.doors.begin(); door != canvas.doors.end(); door++)
        path.addPolygon((*door)->boundPolygon());
    // remove the simplified path (just in case) of doors to be substracted
    shapes = canvas.design->shapes.subtracted(path.simplified()).simplified().toFillPolygons();
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
        if((*door)->getCondition1().isSatisfiable())
            addTransition(pointPair.first, pointPair.second, (*door)->getCondition1(), *door);
        if((*door)->getCondition2().isSatisfiable())
            addTransition(pointPair.second, pointPair.first, (*door)->getCondition2(), *door);
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
            Transition * tr = new Transition(nodeOrigin, nodeDest, (*tran)->door, new SimpleCondition(*(*tran)->condition));
            nodeOrigin->transitions.append(tr);
            this->transitions.append(tr);
        }

    }
    // this will not create a copy of the items, only will append their references
    this->inventory = new Inventory;
    *this->inventory = *other.inventory;
    // current node will be the corresponding copied node of other's current node
    this->current = this->getNode(*other.current);
    this->instances.append(other.instances);
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
        // we get the list of all posible costs to travel through this transition
        Condition::CostList costs = (*tran)->getCost();
        if(costs.isEmpty()){ // if it does not cost anything to perform the transition
            // create a new search state
            DesignGraph * graph = new DesignGraph(*this);
            // insert the door into the instances of the search
            graph->instances.append((*tran)->door);
            // finally, update the transition's condition, so that it remains open only in this new search state
            Transition *aux = graph->getTransition(**tran);
            graph->current = aux->node2;
            delete aux->condition; // transition allways stores conditions in dynamic mem, so delete it
            aux->condition = new SimpleCondition(SimpleCondition::emptyCondition());// create empty cond
            graph->simplify();
            output.append(graph);
        } else {
            for(int i = 0; i < costs.length(); i++){
            //for(auto cost = costs.begin(); cost != costs.end(); cost++){ // iterate through all the possible costs we could spend
                Condition::Cost c(costs.at(i));
                int id = c.first->getId();
                uint number = c.second;
                if(inventory->canAfford(id, number)){ // if we can afford a cost
                    // create a new search state
                    DesignGraph * graph = new DesignGraph(*this);
                    // in this new search state, spend the costs of traveling
                    graph->inventory->spend(id, number);
                    // insert the door into the instances of the search
                    graph->instances.append((*tran)->door);
                    // finally, update the transition's condition, so that it remains open only in this new search state
                    Transition *aux = graph->getTransition(**tran);
                    graph->current = aux->node2;
                    delete aux->condition; // transition allways stores conditions in dynamic mem, so delete it
                    aux->condition = new SimpleCondition(SimpleCondition::emptyCondition());// create empty cond
                    graph->simplify();
                    output.append(graph);
                }
            }
        }
    }
    // second: all the collection of items
    for(int i = 0; i < current->items.length(); i++){
        //create a new search state
        DesignGraph * graph = new DesignGraph(*this);
        //add the collected instance to the list
        graph->instances.append(current->items[i]);
        //add the item to the inventory of the new search state
        graph->inventory->collect(current->items[i]);
        //remove the item from the new search state's current node
        graph->current->items.removeAt(i);
        graph->simplify();
        output.append(graph);
    }
    return output;
}

void DesignGraph::simplify()
{
    QList<QList<RegionNode*>> stronglyConnectedComponents = tarjanAlgorithm();
    for(auto component = stronglyConnectedComponents.begin(); component != stronglyConnectedComponents.end(); component++){
        fuse(*component);
    }
    resetTarjanNodeStates();
}

void DesignGraph::destroyNode(RegionNode *node)
{
    nodes.removeOne(node);
    delete node;
}

double DesignGraph::heuristic() const
{
    static double powerUpWeight = 5;
    static double consumableWeight = 3;
    static double itemWeight = 2;
    static double nodesWeight = 4;

    double output = 1;
    output += inventory->numberOfPowerUps() * powerUpWeight;
    output += inventory->numberOfConsumables() * consumableWeight;
    output += current->items.length() * itemWeight;
    output += current->transitions.length() * nodesWeight;
    output += current->getOpenTransitions().length() * nodesWeight;
    output -= nodes.length() * nodesWeight;
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
    return (*current == *other.current);
}

RegionNode *DesignGraph::getNode(const RegionNode &node) const
{
    for(auto n = nodes.begin(); n != nodes.end(); n++)
        if(**n == node)
            return *n;
    return nullptr;
}

Transition *DesignGraph::getTransition(const Transition &transition) const
{
    for(auto t = transitions.begin(); t != transitions.end(); t++)
        if(**t == transition)
            return *t;
    return nullptr;
}

int DesignGraph::size() const
{
    return nodes.length();
}

bool DesignGraph::isValid() const
{
    return current != nullptr;
}

uint DesignGraph::qHash(const DesignGraph &graph)
{
    return static_cast<uint>(qFloor(graph.heuristic()));
}

void DesignGraph::addTransition(const QPointF &p1, const QPointF &p2, const SimpleCondition &cond, DoorInstance * door)
{
    Transition * tran = new Transition();
    tran->door = door;

    for(auto node = nodes.begin(); node != nodes.end(); node++){

        // check if the node we are iterating through contains one or both of the points of the separation
        if((*node)->containsPoint(p1))
            tran->node1 = *node;
        if ((*node)->containsPoint(p2))
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
        tran->condition = new SimpleCondition(cond); // finally, set the transition condition to a NEW cond
        // is important that the transition contains a new condition because the condition will be exploration-dependent modified
    }
}

void DesignGraph::fuse(QList<RegionNode *> component)
{
    if(component.length() == 1)
        return;
    RegionNode * fused = RegionNode::fusion(component);
    for(int i = 0; i < component.length(); i++){
        // we have to do dirty work to make sure that no transition is un-stable and delete all unused
        if(this->current == component[i])
            this->current = fused;
        component[i]->transitions.clear(); // so that we do not delete a transition twice
        for(int j = transitions.length()-1; j >= 0; j--){
            // if any transition starts in a node that was fused, then delete it.
            if(transitions[j]->node1 == component[i]){
                delete transitions[j];
                transitions.removeAt(j);
            } // otherwise, if it ends AND does not start in a node that was fused, then update the destination to the newly fused node
            else if(transitions[j]->node2 == component[i])
                transitions[j]->node2 = fused;
        }
        destroyNode(component[i]); // destroy nodes from the graph, not the component (memory will be free'd anyways
    }
    nodes.append(fused); // finaly, append the fused node
}

QList<QList<RegionNode *> > DesignGraph::tarjanAlgorithm()
{
    QList<QList<RegionNode *> > output;

    int index = 0;
    QStack<RegionNode *> stack;
    for(auto node = nodes.begin(); node != nodes.end(); node++){
        if((*node)->index == -1)
           output.append(stronglyConnect(*node, stack, index));
    }
    return output;
}

void DesignGraph::resetTarjanNodeStates()
{
    for(auto node = nodes.begin(); node != nodes.end(); node++){
        (*node)->index = -1;
        (*node)->onStack = false;
        (*node)->lowLink = -1;
    }
}

QList<RegionNode *> DesignGraph::stronglyConnect(RegionNode *node, QStack<RegionNode *> &stack, int &index)
{
    node->index = index;
    node->lowLink = index;
    index++;
    stack.push(node);
    node->onStack = true;

    QList<Transition*> openTransitions = node->getOpenTransitions();
    for(auto t = openTransitions.begin(); t != openTransitions.end(); t++){
        RegionNode * endNode = (*t)->node2;
        if(endNode->index == -1){
            stronglyConnect(endNode, stack, index);
            node->lowLink = qMin(node->lowLink, endNode->lowLink);
        } else if(endNode->onStack){
            node->lowLink = qMin(node->lowLink, endNode->index);
        }
    }

    QList<RegionNode *> output;
    if(node->lowLink == node->lowLink){
        RegionNode * aux;
        do{
            aux = stack.pop();
            output.append(aux);
        }while(aux != node);
        // we can compare pointers and not contents since it is an exploration of graph of the same searchState
        // and comparing pointers is way faster than comparing contents
    }
    return output;
}

