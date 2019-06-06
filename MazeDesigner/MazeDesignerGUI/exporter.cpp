#include "exporter.h"

Exporter::Exporter()
{

}

void Exporter::load(QFile *project)
{
    this->project = project;
}

void Exporter::exportDesign(const Canvas &canvas) const
{

}
