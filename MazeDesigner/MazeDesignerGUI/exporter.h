#ifndef EXPORTER_H
#define EXPORTER_H

#include "../Common/common.h"
#include "../Canvas/canvas.h"

class Exporter
{
public:
    Exporter();
    void load(QFile *project);
    void exportDesign(const Canvas & canvas) const;
private:
    QFile * project;

    QJsonObject objWall;
    QString objWallName = "obj_wall";
};

#endif // EXPORTER_H
