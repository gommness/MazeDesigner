#include "doorinstance.h"

int DoorInstance::ID = 0;

DoorInstance::DoorInstance(QLineF &line) : QLineF (line)
{
    DoorInstance::ID++;
    id = DoorInstance::ID;
    area = lineArea(line);
}

DoorInstance::DoorInstance(DoorInstance &other) : QLineF (other)
{
    id = other.id;
    area = other.area;
    condition1 = other.condition1;
    condition2 = other.condition2;
}

DoorInstance::~DoorInstance()
{
    delete condition1;
    delete condition2;
}

QPolygonF DoorInstance::boundPolygon() const
{
    return QPolygonF(area);
}

QString DoorInstance::instanceInfo() const
{
    QString output;
    QString c1, c2;
    if(condition1 != nullptr)
        c1 = condition1->toString();
    if(condition2 != nullptr)
        c2 = condition2->toString();
    output += "door " + QString::number(id) + " with condition\nfrom A to B:{"+c1+
            "}\nfrom B to A:{"+c2+"}";
    TODO("insertar el string de la condicion en la puerta cuando este implementado");
    return output;
}

QPointF DoorInstance::instancePosition() const
{
    return this->center();
}

bool DoorInstance::contains(const QPointF &point) const
{
    return area.containsPoint(point, Qt::FillRule::OddEvenFill);
}

DoorInstance::DoorInstance(const QJsonObject &json)
{
    int id;
    QLineF line;
    if(json.contains("ID") && json["ID"].isDouble()){
        id = json["ID"].toInt();
    } else {
        throw std::runtime_error("no id found for door instance in jsonObject");
    }

    if(json.contains("point1") && json["point1"].isString()
            && json.contains("point2") && json["point2"].isString()){
        QPointF p1 = json::parsePoint(json["point1"].toString());
        QPointF p2 = json::parsePoint(json["point2"].toString());
        line = QLineF(p1, p2);
    } else {
        throw std::runtime_error("missing points for door instance in jsonObject");
    }

    if(json.contains("condition1") && json["condition1"].isObject()){
        condition1 = new SimpleCondition(json["condition1"].toObject());
    } else {
        throw std::runtime_error("missing condition for door instance in jsonObject");
    }
    if(json.contains("condition2") && json["condition2"].isObject()){
        condition2 = new SimpleCondition(json["condition2"].toObject());
    } else {
        delete condition1;
        throw std::runtime_error("missing condition for door instance in jsonObject");
    }
    this->id = id;
    this->setPoints(line.p1(), line.p2());
    this->area = lineArea(line);
}

QJsonObject DoorInstance::toJson() const
{
    QJsonObject output;
    output.insert("ID", id);
    output.insert("point1", QString(POINTFORMAT).arg(p1().x()).arg(p1().y()));
    output.insert("point2", QString(POINTFORMAT).arg(p2().x()).arg(p2().y()));
    if(condition1 != nullptr)
        output.insert("condition1", condition1->toJson());
    else
        output.insert("condition1", SimpleCondition::emptyCondition().toJson());
    if(condition2 != nullptr)
        output.insert("condition2", condition2->toJson());
    else
        output.insert("condition2", SimpleCondition::emptyCondition().toJson());
    return output;
}

QPair<QPointF, QPointF> DoorInstance::separation() const
{
    QPair<QPointF, QPointF> output;
    QLineF normal(normalVector());
    //translate normal to (0,0) so it acts as a vector
    normal.setP1(QPointF(0,0));
    normal.setP2(normal.p2()-this->p1());

    normal.setLength(8);
    output.first = center()+normal.p2();
    normal.setLength(-8);
    output.second = center()+normal.p2();
    return output;
}

SimpleCondition DoorInstance::getCondition1() const
{
    if(condition1 != nullptr)
        return *condition1;
    else
        return SimpleCondition::emptyCondition();
}

SimpleCondition DoorInstance::getCondition2() const
{
    if(condition2 != nullptr)
        return *condition2;
    else
        return SimpleCondition::emptyCondition();
}

void DoorInstance::drawSelf(QPainter &painter, QPointF offset) const
{

    painter.drawLine(this->translated(offset));
    QPair<QPointF, QPointF> points = separation();
    QPointF areaPoint(4,8);
    QRectF bound(points.first-areaPoint, points.first+areaPoint);
    painter.drawText(bound.translated(offset), Qt::AlignCenter, "A");
    bound = QRectF(points.second-areaPoint, points.second+areaPoint);
    painter.drawText(bound.translated(offset), Qt::AlignCenter, "B");
}

void DoorInstance::onCondition1Changed(SimpleCondition &cond)
{
    delete condition1;
    condition1 = new SimpleCondition(cond);
}

void DoorInstance::onCondition2Changed(SimpleCondition &cond)
{
    delete condition2;
    condition2 = new SimpleCondition(cond);
}

DoorInstance::DoorInstance(int id, QLineF &line) : QLineF (line)
{
    this->id = id;
}

QPolygonF DoorInstance::lineArea(QLineF line)
{
    QVector<QPointF> poly;
    QLineF normal = line.normalVector();
    // normalVector, despite the name, returns a orthogonal line starting at the same point as line
    // thus, we need to translate that normalLines to the origin, maintaining the vector
    normal.setP1(QPointF(0,0));
    normal.setP2(normal.p2()-line.p1());
    // then adjust the length of the line for area purposes
    normal.setLength(4);
    // and finally construct the polygon surrounding the line
    poly.append(line.p1()+normal.p2());
    poly.append(line.p2()+normal.p2());
    normal.setLength(-4); // we invert the length here to capture the other side
    poly.append(line.p2()+normal.p2());
    poly.append(line.p1()+normal.p2());
    return QPolygonF(poly);
}

