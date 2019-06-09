#include "regionnode.h"

RegionNode::RegionNode(QPolygonF *parent) : parent(parent) {}

RegionNode::~RegionNode() {}

bool RegionNode::operator ==(const RegionNode &other)
{
    return this->parent == other.parent;
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
    return (key.items.length()+1)*(key.transitions.length()+1);
}

Transition::Transition(RegionNode *node1, RegionNode *node2, Condition *cond) : node1(node1), node2(node2), condition(cond)
{

}

bool Transition::isValid() const
{
    return node1 != nullptr && node2 != nullptr && (condition == nullptr || condition->validate());
}
