#include "exporter.h"

// Class's roomTemplate initialization:
const QJsonDocument Exporter::roomTemplate = QJsonDocument::fromBinaryData(QFile(TEMPLATEPATH).readAll());

Exporter::Exporter()
{

}

void Exporter::load(QFile *project)
{
    this->project = project;
}

void Exporter::exportDesign(const Canvas &canvas, const RoomCanvas & roomCanvas) const
{

}
