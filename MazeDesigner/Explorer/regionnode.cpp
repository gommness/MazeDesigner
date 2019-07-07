#include "regionnode.h"

RegionNode::RegionNode(QPolygonF *parent) {
    parents.insert(parent);
    //parents.append(parent);
}

RegionNode::RegionNode(RegionNode &other)
{
    parents.unite(other.parents);
    for(auto key = other.items.begin(); key != other.items.end(); key++){
        // we do not create copies of the instances, since we only care for the references
        this->items.append(*key);
    }
    // README we do not set the transitions as they have to be set from the outside.
    // this is because, we will be copying the nodes in order to rebuild a graph.
    // since we want this copied node to be connected to other copied nodes, we will not
    // set its transitions now, because if we did, this new-graph node will transition to old-graph nodes
}

RegionNode::~RegionNode() {
    for(int i = transitions.length()-1; i >= 0; i--)
        delete transitions[i];
}

bool RegionNode::operator ==(const RegionNode &other) const
{
    if(this->parents.size() != other.parents.size())
        return false;
    //bool output;//  = this->parents == other.parents;
    auto parentList = parents.toList();
    auto otherParents = other.parents.toList();
    for(int i = 0; i < parentList.size(); i++)
        if(*parentList[i] != *otherParents[i])
            return false;

    // README watch out. if the list are not ordered in the same way, then they are considered not equal
    // also, remember that the KeyInstances stored in our lists are allways the same, thus reference equallity is good enough
    return this->items == other.items;
    //return output;
}

bool RegionNode::operator !=(const RegionNode &other) const
{
    return !(*this == other);
}

RegionNode *RegionNode::fusion(QList<RegionNode *> &nodes)
{
    QPolygonF region;
    RegionNode * output = new RegionNode(&region);
    QList<Transition*> ts; // auxiliary transition list.
    // create the union bewtween the polygons of the fused nodes
    // give all the items contained in nodes to the fused node, since it will have the items from every node
    // store all transitions starting from any of the nodes, in the auxiliary list
    for(auto node = nodes.begin(); node != nodes.end(); node++){
        qDebug() << (*node)->parents.isEmpty();
        output->parents.unite((*node)->parents);
        //output->parents.append((*node)->parents);
        //region = (*node)->parent->united(region);
        output->items.append((*node)->items);
        ts.append((*node)->transitions);
    }
    // now we'll prepare all the transitions for the fused node
    for(int i = 0; i < ts.length(); i++){
        // for each transition (starting in any of the nodes to be fused) create a new one
        // the starting point of the new transition will be the fused node anyways so don't worry
        // now if the transition ended in a node of the list, we'd have to modify the transition so that it ends in the fused node
        // note that the end points of the transition might end in a different third node, thus we HAVE to make this comparation
        RegionNode * dest;
        if(nodes.contains(ts[i]->node2)){ // if the destination node (2) of the previous transition contains a node in our list
            dest = output; // then the destination node is none other than this newly created fused node
        } else {
            dest = ts[i]->node2; // if it is some other node, then just copy that
        }
        SimpleCondition * newCond = new SimpleCondition(*ts[i]->condition); // copy the condition into a new one
        // create the transition and insert it into the transitions of the fused node
        output->transitions.append(new Transition(output, dest, ts[i]->door, newCond));
    }
    return output;
}

bool RegionNode::containsPoint(const QPointF &point) const
{
    bool output = false;
    for(auto poly = parents.begin(); poly != parents.end(); poly++){
        output = (*poly)->containsPoint(point, Qt::FillRule::OddEvenFill);
        if(output == false)
            return false;
    }
    return output;
}

QList<Transition *> RegionNode::getOpenTransitions() const
{
    QList<Transition*> output;
    for(auto t = transitions.begin(); t != transitions.end(); t++)
        if((*t)->isOpen())
            output.append(*t);
    return output;
}

uint RegionNode::qHash(const RegionNode &key)
{
    return (static_cast<uint>(key.items.length())+1)*(static_cast<uint>(key.transitions.length())+1);
}

Transition::Transition(RegionNode *node1, RegionNode *node2, DoorInstance *door, SimpleCondition * cond)
    : node1(node1), node2(node2), door(door), condition(cond)
{

}

Transition::~Transition()
{
    delete condition;
}

bool Transition::isValid() const
{
    return node1 != nullptr && node2 != nullptr && (condition == nullptr || condition->validate());
}

bool Transition::isOpen() const
{
    return condition == nullptr || condition->isEmpty();
}

Condition::CostList Transition::getCost() const
{
    if(condition == nullptr)
        return Condition::CostList();
    return condition->getCost();
}

bool Transition::operator ==(const Transition &other) const
{
    // README remember to check equality on content and not pointers. this is actually very important
    bool output;
    if(this->condition == nullptr && other.condition == nullptr)
        output = true;
    else if(this->condition != nullptr && other.condition != nullptr)
        output = *this->condition == *other.condition;
    else
        return false;
    qDebug()<<"-----------------------TRANSITION COMPARATION----------------------------------";
    qDebug() << "condition"<<this->condition<<"equal to"<<other.condition<<"?"<<output;
    output &= (!this->node1 && !other.node1)||(this->node1 && other.node1 && *this->node1 == *other.node1);
    qDebug() << "node"<<node1<<"equal to"<<other.node1<<"?"<<output;
    output &= (!this->node2 && !other.node2)||(this->node2 && other.node2 && *this->node2 == *other.node2);
    qDebug() << "node"<<node2<<"equal to"<<other.node2<<"?"<<output;
    qDebug() <<"is transition"<<this<<" equalto "<<&other<<"???"<<output;
    qDebug()<<"...............................................................................";
    return output;
}

bool Transition::operator !=(const Transition &other) const
{
    return !(*this == other);
}
