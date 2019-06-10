#include "exporter.h"
#include <QUuid>
#include <QJsonArray>
#include <QRandomGenerator>
#include <sstream>

// Class's json templates initialization:
const QJsonDocument Exporter::roomTemplate = QJsonDocument::fromJson(readFile(ROOMTEMPLATEPATH));
const QJsonDocument Exporter::roomResourceTemplate = QJsonDocument::fromJson(readFile(ROOMRESOURCETEMPLATE));
const QJsonObject Exporter::solidInstanceTemplate = QJsonDocument::fromJson(readFile(INSTANCETEMPLATEPATH)).object();
const QString Exporter::objWallName = "obj_wall";

void Exporter::exportDesign(const QString & filename, const Canvas &canvas, const RoomCanvas & roomCanvas)
{
    TODO(Exporter::roomTemplate.toJson().toStdString());
    if(filename.isEmpty())
        return;

    QPainterPath design = canvas.getDesign();
    QString objId = getOrCreateObject(filename, Exporter::objWallName);
    QFileInfo file(filename);
    for(auto room = roomCanvas.roomList.begin(); room != roomCanvas.roomList.end(); room++){ // loop through all rooms
        if(room->isExportable()){
            QJsonObject obj = roomToGameMakerRoom(objId, canvas.grid.getSize(), design, *room);
            QJsonDocument jDoc(obj);
            QString roomFolderPath = file.path()+QDir::separator()+"rooms"+QDir::separator()+room->getName();
            //if the directory does not exist, we must create it before creating the room
            if(!QDir(roomFolderPath).exists()){
                QDir().mkdir(roomFolderPath);
            }
            QFile roomFile(roomFolderPath+QDir::separator()+room->getName()+".yy");
            roomFile.open(QIODevice::WriteOnly);
            roomFile.write(jDoc.toJson());
            roomFile.close();
            insertRoomInProject(filename, obj["id"].toString(), room->getName());
        }
    }
}

QList<QRect> Exporter::quadify(const QPolygonF &poly)
{
    QList<QRect> output;
    QRegion reg(poly.toPolygon());
    for(auto rect = reg.begin(); rect != reg.end(); rect++)
        output.append(*rect);
    return output;
}

QList<QRect> Exporter::quadify(const QPainterPath & design, const Room &room)
{
    QPainterPath intersection;
    intersection.addRect(room);
    intersection = intersection.subtracted(design).simplified();
    QList<QPolygonF> polys = intersection.toFillPolygons();
    QList<QRect> output;
    for(auto poly = polys.begin(); poly != polys.end(); poly++){
        output.append(quadify(*poly));
    }
    return output;
}

QJsonObject Exporter::roomToGameMakerRoom(const QString & objId,  const int &gridSize, const QPainterPath &design, const Room &room)
{
    QJsonObject output = QJsonObject(Exporter::roomTemplate.object());
    QList<QRect> solids = quadify(design, room);
    QJsonObject tmp;
    QJsonArray tmpArray;

    output["name"] = room.getName();
    output["id"] = UUID();

    tmp = output["physicsSettings"].toObject();
    tmp["id"] = UUID();
    output["physicsSettings"] = tmp;

    tmp = output["roomSettings"].toObject();
    tmp["id"] = UUID();
    tmp["Height"] = cellHeight * room.height()/gridSize;
    tmp["Width"] = cellWidth * room.width()/gridSize;
    output["roomSettings"] = tmp;

    // inserting the views
    QJsonArray viewsArray = output["views"].toArray();
    for(auto view = viewsArray.begin(); view != viewsArray.end(); view++){
        tmp = view->toObject();
        tmp["id"] = UUID();
        tmpArray.append(tmp);
    }
    output["views"] = tmpArray;

    tmp = output["viewSettings"].toObject();
    tmp["id"] = UUID();
    output["viewSettings"] = tmp;

    // inserting the layers
    QList<QString> instanceUuids;
    tmpArray = QJsonArray();
    QJsonArray layersArray = output["layers"].toArray();
    for(auto layer = layersArray.begin(); layer != layersArray.end(); layer++){
        tmp = layer->toObject();
        tmp["id"] = UUID();
        if(tmp["__type"] == LAYERINSTANCETYPE && tmp["name"] == SOLIDSLAYERNAME){ //if the layer is the solids layer
            QList<QRect> solids = quadify(design, room);
            QJsonArray jsonSolids;
            for(auto solid = solids.begin(); solid != solids.end(); solid++){
                QString uuid = UUID();
                instanceUuids.append(uuid);
                jsonSolids.append(createSolidInstance(uuid, objId, gridSize, *solid, QPoint(room.x(), room.y())));
            }
            tmp["instances"] = jsonSolids;
        }
        tmpArray.append(tmp);
    }
    output["layers"] = tmpArray;

    tmpArray = QJsonArray();
    for(auto uuid = instanceUuids.begin(); uuid != instanceUuids.end(); uuid++){
        tmpArray.append(*uuid);
    }
    output["instanceCreationOrderIDs"] = tmpArray;

    return output;
}

QJsonObject Exporter::createSolidInstance(const QString & instanceId, const QString &objId,
                                          const int &gridSize, const QRect & rect, const QPoint & origin)
{
    QJsonObject output = QJsonObject(Exporter::solidInstanceTemplate);
    output["id"] = instanceId;
    output["objId"] = objId;
    //obtain random name for instance
    std::stringstream sstream;
    sstream << std::hex << QRandomGenerator::global()->generate();
    std::string result = sstream.str();
    QString name = "inst_" + QString::fromStdString(result);
    qDebug() << "grid size: "<<gridSize;
    output["x"] = cellWidth*(rect.x()-origin.x())/gridSize;
    output["y"] = cellHeight*(rect.y()-origin.y())/gridSize;
    output["scaleX"] = rect.width()/gridSize;
    output["scaleY"] = rect.height()/gridSize;
    output["name"] = name;
    output["name_with_no_file_rename"] = name;
    return output;
}

void Exporter::insertRoomInProject(const QString &projectPath, const QString &uuid, const QString &roomName)
{
    QJsonObject project = QJsonDocument::fromJson(readFile(projectPath)).object();
    QJsonArray resources = project["resources"].toArray();
    QJsonObject content;
    for(auto res = resources.begin(); res != resources.end(); res++){
        content = (*res).toObject();
        if(content["Key"] == uuid){
            return;
        }
    }
    QJsonObject room(Exporter::roomResourceTemplate.object());
    room.insert("Key", uuid);
    QJsonObject value;
    value.insert("id", UUID());
    value.insert("resourcePath", "rooms\\"+roomName+"\\"+roomName+".yy");
    value.insert("resourceType", "GMRoom");
    room.insert("Value", value);
    resources.append(room);
    project["resources"] = resources;
    QJsonDocument output(project);
    QFile projectFile(projectPath);
    projectFile.open(QIODevice::WriteOnly);
    projectFile.write(output.toJson());
    projectFile.close();
}

bool Exporter::copyRecursively(QString sourceFolder, QString destFolder)
{
    bool success = false;
    QDir sourceDir(sourceFolder);

    if(!sourceDir.exists())
        return false;

    QDir destDir(destFolder);
    if(!destDir.exists())
        destDir.mkdir(destFolder);

    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++) {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        success = QFile::copy(srcName, destName);
        if(!success)
            return false;
    }

    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        success = copyRecursively(srcName, destName);
        if(!success)
            return false;
    }

    return true;
}

QString Exporter::getOrCreateObject(const QString &filename, const QString &objName)
{
    QString output;
    QFile project(filename);
    project.open(QIODevice::ReadWrite);
    QString content = project.readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject json = jDoc.object();

    QJsonArray resources = json["resources"].toArray();
    QString expectedResPath = "objects\\"+objName+"\\"+objName+".yy";
    for(auto res = resources.begin(); res != resources.end(); res++){
        QJsonObject obj = res->toObject();
        //if the project already had a obj_wall, then we return its uuid
        if(obj["Value"].toObject()["resourcePath"].toString() == expectedResPath){
            project.close();
            return obj["Key"].toString();
        }
    }
    //if we make it out of the loop, then the project did not have a obj_wall and we'll create it
    //first, we declare the resource in the project file
    output = UUID();
    QJsonObject obj;
    obj["Key"] = output;
    QJsonObject value;
    value["resourcePath"] = QString("objects")+QDir::separator()+objName+QDir::separator()+objName+".yy";
    value["id"] = getObjTemplateID();
    value["resourceType"] = "GMObject";
    obj["Value"] = value;
    // then, we copy our templates directly into the project
    QFileInfo fInfo(project);
    copyRecursively(RESOBJWALL, fInfo.path()+QDir::separator()+"objects");
    copyRecursively(RESSPRWALL, fInfo.path()+QDir::separator()+"sprites");
    project.close();

    return output;
}

QString Exporter::getObjTemplateID()
{
    QString output;
    QFile file(RESOBJWALLFILE);
    file.open(QIODevice::ReadOnly);
    QString content = file.readAll();
    QJsonObject obj = QJsonDocument::fromJson(content.toUtf8()).object();
    output = obj["id"].toString();
    file.close();
    return output;
}

QByteArray Exporter::readFile(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray output = file.readAll();
    file.close();
    return output;
}
