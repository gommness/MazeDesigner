#ifndef EXPORTER_H
#define EXPORTER_H

#include "../Common/common.h"
#include "../Canvas/canvas.h"
#include "../Canvas/roomcanvas.h"
#include <QJsonDocument>

#define TEMPLATEPATH "../roomTemplate.yy"

class Exporter
{
public:
    Exporter();
    void load(QFile *project);
    void exportDesign(const Canvas & canvas, const RoomCanvas & roomCanvas) const;
private:
    static const QJsonDocument roomTemplate;

    QFile * project;
    QJsonObject objWall;
    QString objWallName = "obj_wall";
};

#endif // EXPORTER_H
