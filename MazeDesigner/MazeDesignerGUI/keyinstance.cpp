#include "keyinstance.h"
#include <QRegExp>
#include <QStringList>

int KeyInstance::ID;

KeyInstance::KeyInstance(const Key & model, const qreal & x, const qreal & y) : QRectF(x-8,y-8,16,16), model(model){
    KeyInstance::ID++;
    id = KeyInstance::ID;
}

KeyInstance::KeyInstance(const Key &model, const QPointF & point) : QRectF(point.x()-8,point.y()-8,16,16),
    model(model)
{
    KeyInstance::ID++;
    id = KeyInstance::ID;
}

KeyInstance::KeyInstance(KeyInstance &other)
{
    *this = other;
}

KeyInstance &KeyInstance::operator =(KeyInstance &other)
{
    this->id = other.id;
    this->model = other.model;
    qreal x1,y1,x2,y2;
    other.getCoords(&x1, &y1, &x2, &y2);
    this->setCoords(x1,y1,x2,y2);
    return *this;
}

QPolygonF KeyInstance::boundPolygon() const
{
    return QPolygonF(*this);
}

QString KeyInstance::instanceInfo() const
{
    QString output;
    output += "key " + QString::number(id) + " instance of " + model.getName();
    return output;
}

QPointF KeyInstance::instancePosition() const
{
    return this->center();
}

KeyInstance::KeyInstance(const QJsonObject &json, const KeyRepository & repo)
{
    int jsonId;
    Key model;
    QPointF coords;
    if(json.contains("ID") && json["ID"].isDouble()){
        jsonId = json["ID"].toInt();
    } else {
        throw std::runtime_error("no id for key instance found in jsonObject");
    }
    if(json.contains("model") && json["model"].isDouble()){
        model = repo.keyWithId(json["model"].toInt());
        if(!model.isValid())
            throw std::runtime_error("model for key instance found in jsonObject does not exist");
    } else {
        throw std::runtime_error("no model for key instance found in jsonObject");
    }
    if(json.contains("coords") && json["coords"].isString()){
        QRegExp regex(POINTSPLITREGEX);
        QStringList splitted = json["coords"].toString().split(regex, QString::SplitBehavior::SkipEmptyParts);
        if(splitted.size() < 2) // if there is less than 2 doubles, it ain't no point pal!
            throw std::runtime_error("error while parsing point: "+json["coords"].toString().toUtf8()); // we'll panic
        coords = QPointF(splitted[0].toDouble(), splitted[1].toDouble());
    } else {
        throw std::runtime_error("no coords for key instance found in jsonObject");
    }
    this->id = jsonId;
    this->model = model;
    //x-8,y-8,16,16
    this->setCoordsFromPoint(coords);
}

QJsonObject KeyInstance::toJson()
{
    QJsonObject output;
    output.insert("ID", id);
    output.insert("model", model.getId());
    output.insert("coords", QString(POINTFORMAT).arg(center().x()).arg(center().y()));
    return output;
}

int KeyInstance::getId()
{
    return id;
}

QString KeyInstance::toString() const
{
    QString output = QString("KEY{")+QString::number(id)+", ("+model.toString()+")}";
    return output;
}

const Key KeyInstance::getModel() const
{
    return model;
}

KeyInstance::KeyInstance(int id, const Key &model, const qreal &x, const qreal &y) : QRectF(x-8,y-8,16,16), model(model)
{
    this->id = id;
}

KeyInstance::KeyInstance(int id, const Key &model, const QPointF &point) : QRectF(point.x(),point.y(),16,16),
    model(model)
{
    this->id = id;
}

void KeyInstance::setCoordsFromPoint(const QPointF &point)
{
    this->setCoords(point.x()-8, point.y()-8, point.x()+8, point.y()+8);
}
