#include "doorinstance.h"

int DoorInstance::ID = 0;

DoorInstance::DoorInstance(QLineF &line) : QLineF (line)
{
    DoorInstance::ID++;
    id = DoorInstance::ID;
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
    area = QPolygonF(poly);
}

QPolygonF DoorInstance::boundPolygon() const
{
    return QPolygonF(area);
}

QString DoorInstance::instanceInfo() const
{
    QString output;
    output += "door " + QString::number(id) + " with condition: {}";
    TODO("insertar el string de la condicion en la puerta cuando este implementado");
    return output;
}

bool DoorInstance::contains(const QPointF &point) const
{
    return area.containsPoint(point, Qt::FillRule::OddEvenFill);
}

DoorInstance DoorInstance::fromJson(const QJsonObject &json)
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
    TODO("desserialize the door conditions when they are implemented");
    return DoorInstance(id, line);
}

QJsonObject DoorInstance::toJson() const
{
    QJsonObject output;
    output.insert("ID", id);
    output.insert("point1", QString(POINTFORMAT).arg(p1().x()).arg(p1().y()));
    output.insert("point2", QString(POINTFORMAT).arg(p2().x()).arg(p2().y()));
    TODO("serialize the door conditions when they are implemented");
    return output;
}

DoorInstance::DoorInstance(int id, QLineF &line) : QLineF (line)
{
    this->id = id;
}

