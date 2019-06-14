#include "regionnode.h"

RegionNode::RegionNode(QPolygonF *parent) : parent(parent) {}

RegionNode::RegionNode(RegionNode &other)
{
    parent = other.parent;
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
    bool output = this->parent == other.parent;
    // README watch out. if the list are not ordered in the same way, then they are considered not equal
    // also, remember that the KeyInstances stored in our lists are allways the same, thus reference equallity is good enough
    output &= this->items == other.items;
    return output;
}

bool RegionNode::operator !=(const RegionNode &other) const
{
    return !(*this == other);
}

RegionNode *RegionNode::fusion(const RegionNode &n1, const RegionNode &n2)
{
    // create the union bewtween the two polygons of the fused nodes
    QPolygonF region = QPolygonF(n1.parent->united(*n2.parent));
    RegionNode * output = new RegionNode(&region);
    // give all the items contained in both nodes to the fused node, since it will have both items from n1 and from n2
    output->items.append(n1.items);
    output->items.append(n2.items);
    QList<Transition*> ts; // auxiliary transition list.
    // store all transitions, starting either in n1 or n2.
    ts.append(n1.transitions);
    ts.append(n2.transitions);
    // now we'll prepare all the transitions for the fused node
    for(int i = 0; i < ts.length(); i++){
        // for each transition (either starting in n1 or n2) create a new one
        // the starting point of the new transition will be the fused node anyways so don't worry
        // now if the transition ended in either n1 or n2, we'd have to modify the transition so that it ends in the fused node
        // note that the end points of the transition might end in a different third node, thus we HAVE to make this comparation
        RegionNode * dest;
        if(*ts[i]->node2 == n1 || *ts[i]->node2 == n2){
            dest = output;
        } else {
            dest = ts[i]->node2;
        }
        SimpleCondition * newCond = new SimpleCondition(*ts[i]->condition); // copy the condition into a new one
        output->transitions.append(new Transition(output, dest, newCond)); // create the transition and insert it into the transitions of the fused node
    }
    return output;
}

bool RegionNode::containsPoint(const QPointF &point) const
{
    return parent->containsPoint(point, Qt::FillRule::OddEvenFill);
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

Transition::Transition(RegionNode *node1, RegionNode *node2, SimpleCondition *cond) : node1(node1), node2(node2), condition(cond)
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
    bool output = *this->condition == *other.condition;
    output &= *this->node1 == *other.node1;
    output &= *this->node2 == *other.node2;
    return output;
}

bool Transition::operator !=(const Transition &other) const
{
    return !(*this == other);
}
