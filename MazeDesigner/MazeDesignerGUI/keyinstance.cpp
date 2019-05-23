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

QString KeyInstance::instanceInfo() const
{
    QString output;
    output += "key " + QString::number(id) + " instance of " + model.getName();
    return output;
}

KeyInstance KeyInstance::fromJson(const QJsonObject &json, const KeyRepository & repo)
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
    KeyInstance output(jsonId, model, coords);
    return output;
}

QJsonObject KeyInstance::toJson()
{
    QJsonObject output;
    output.insert("ID", id);
    output.insert("model", model.getId());
    output.insert("coords", QString(POINTFORMAT).arg(x()).arg(y()));
    return output;
}

int KeyInstance::getId()
{
    return id;
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
