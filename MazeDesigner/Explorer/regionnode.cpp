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

RegionNode *RegionNode::fusion(RegionNode &n1, RegionNode &n2)
{
    QPolygonF region = QPolygonF(n1.parent->united(*n2.parent));
    RegionNode * output = new RegionNode(&region);
    output->items.append(n1.items);
    output->items.append(n2.items);
    output->transitions.append(n1.transitions);
    output->transitions.append(n2.transitions);
    for(int i = 0; i < output->transitions.length(); i++){
        if(*output->transitions[i]->node1 == n1 || *output->transitions[i]->node1 == n2){
            output->transitions[i]->node1 = output;
        }
        if(*output->transitions[i]->node2 == n1 || *output->transitions[i]->node2 == n2){
            output->transitions[i]->node2 = output;
        }
    }
    return output;
}

bool RegionNode::containsPoint(const QPointF &point) const
{
    return parent->containsPoint(point, Qt::FillRule::OddEvenFill);
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
