#ifndef EXPORTER_H
#define EXPORTER_H

#include "../Common/common.h"
#include "../Canvas/canvas.h"
#include "../Canvas/roomcanvas.h"
#include <QJsonDocument>
#include <QDir>

#define ROOMTEMPLATEPATH "../res/roomTemplate.yy"
#define INSTANCETEMPLATEPATH "../res/solidInstanceTemplate.json"
#define RESOBJWALL "../res/obj_wall"
#define RESOBJWALLFILE "../res/obj_wall/obj_wall.yy"
#define RESSPRWALL "../res/spr_wall"
#define SOLIDSLAYERNAME "solids"

#define LAYERINSTANCETYPE "GMRInstanceLayer_Model:#YoYoStudio.MVCFormat"
#define UUID() QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces)

class Exporter
{
public:
    Exporter(){}
    static void exportDesign(const QString & filename, const Canvas & canvas, const RoomCanvas & roomCanvas);
private:
    static const QJsonDocument roomTemplate;
    static const QJsonObject solidInstanceTemplate;
    static const QString objWallName;
    static const int cellHeight = 32;
    static const int cellWidth = 32;

    static QList<QRect> quadify(const QPolygonF & poly);
    static QList<QRect> quadify(const QPainterPath & design, const Room & room);
    static QJsonObject roomToGameMakerRoom(const QString & objId, const int & gridSize, const QPainterPath & design,
                                           const Room & room);
    static QJsonObject createSolidInstance(const QString & instanceId, const QString & objId, const int & gridSize,
                                           const QRect & rect,  const QPoint & origin);
    static bool copyRecursively(QString sourceFolder, QString destFolder);
    static QString getOrCreateObject(const QString & project, const QString & objName);
    static QString getObjTemplateID();
    static QByteArray readFile(const QString & file);

};

#endif // EXPORTER_H
