#include "canvas.h"
#include <QPainter>
#include <QDebug>
#include <QLineF>
#include <cstdlib>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QVariant>
#include <QJsonArray>
#include <QJsonDocument>
#include <stdexcept>

#define UMBRAL 0.1

Canvas::Canvas(QWidget *parent) : QWidget (parent), grid(this)
{
    //this->setSizePolicy(QSizePolicy::Maximum);
    transformed = false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    QHBoxLayout *aux = new QHBoxLayout;
    aux->setMargin(0);
    aux->addWidget(&grid);
    this->setLayout(aux);

    //polyList = QList<QPolygon>();
    grid.setSize(36);
    /*
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(this);
    */
    //painter.begin(this);
    //painter.setPen(QColor(0,0,0));
    //painter.setRenderHint(QPainter::Antialiasing);
    //qDebug() << "painter active? " << painter.isActive();
}

Canvas::~Canvas()
{
    delete start;
    delete end;
    start = end = nullptr;
}

QSize Canvas::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Canvas::sizeHint() const
{
    return QSize(4000, 2000);
}

void Canvas::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void Canvas::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

QString Canvas::toString() const
{
    QString output("");
    /*
    foreach(QPolygon poly, polyList){
        output += "polygon : { ";
        foreach(QPoint coord, poly){
            output += "Point: {x: "+QString::number(coord.x()) +" y: "+QString::number(coord.y()) +"} ";
        }
        output += " }\n";
    }
    */
    return output;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    //grid.update(); // child widget's update calls father's update!
    painter.begin(this);
    QPen pen;
    pen.setColor(QColor(0,0,0));
    pen.setWidth(4);
    painter.setPen(pen);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(grid.getScale(), grid.getScale());
    render();
    painter.end();

    delete start;
    delete end;
    start = end = nullptr;
}



void Canvas::mousePressEvent(QMouseEvent *event)
{
    grid.mousePressEventHandler(event);
    delete start;
    start = new QPointF(grid.nearestPoint(event->pos()));
}


void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    grid.mouseMoveEventHandler(event);
}

/*
 * TODO draw rectangle that traces the movement of the mouse. Might need another "canvas" to do this
void Canvas::mouseMoveEvent(QMouseEvent *event){
    if(start != nullptr){
        QRect rect(*start, nearestGridPoint(event->pos()));
        update(rect.normalized());
    }
}
*/

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    grid.mouseReleaseEventHandler(event);
    delete end;
    end = new QPointF(grid.nearestPoint(event->pos()));
    qDebug() << "recieved mouse elease event. end: " << end->x() << " " << end->y();
    if(start != nullptr && start != end
            && std::abs(start->x() - end->x()) > UMBRAL
            && std::abs(start->y() - end->y()) > UMBRAL){
        QPolygonF rect = QRectF(*start, *end).normalized();
        rect.append(*rect.begin());
        // this line is necessary because QRect is a vector of only 4 QPoints
        // while a QPolygon would have an extra 5th QPoint that equals to the first QPoint
        // By doing this, we build the rectangle as a polygon so that future extractions and subtractions are performed correctly
        qDebug() << "creando el poligono: " << rect;
        if(event->button() & Qt::LeftButton){ // left click
            addPolygon(&rect);
        } else if(event->button() & Qt::RightButton){ // right click
            removePolygon(&rect);
        }
        update();
    }
}

void Canvas::addPolygon(QPolygonF *other)
{
    QPainterPath adding;
    adding.addPolygon(*other);
    shapes = shapes.united(adding).simplified();
    //shapes.addPolygon(*other);
    //addToPolyList(polyList, other);
}

void Canvas::addHole(QPolygonF *other)
{
    QPainterPath removing;
    removing.addPolygon(*other);
    shapes = shapes.subtracted(removing).simplified();
    //addToPolyList(holeList, other);
}

void Canvas::removePolygon(QPolygonF *other)
{
    QPainterPath removing;
    removing.addPolygon(*other);
    shapes = shapes.subtracted(removing).simplified();
    /*
    for(auto poly = polyList.begin(); poly != polyList.end(); poly++){
        if(poly->intersects(*other)){
            qDebug() << "igualdad de " << poly->united(*other) << " y " << *poly << " es: " << (poly->united(*other) == *poly);
            if(poly->united(*other) == *poly){ // if A union B is equal to A, then B is subset of A
                qDebug() << "SI contiene al poligono a quitar";
                // addHole(other);
            } else {
                qDebug() << "no contiene al poligono a quitar";
                *poly = poly->subtracted(*other);
                //return;
            }
        }
    }
    */
}

void Canvas::render()
{
    // render the polygons
    // OPTIMIZEME translation not optimal but not going to bother FOR NOW
    painter.drawPath(shapes.translated(grid.getOffset()));
    painter.fillPath(shapes.translated(grid.getOffset()), QBrush(QColor(200,200,200)));
    //painter.drawPath(shapes.translated(grid.getOffset()));
    /*
    QList<QPolygonF> polys = shapes.toFillPolygons();
    for(auto poly = polys.begin(); poly != polys.end(); poly++)
        painter.drawPolygon(*poly);
    */
    /*
    foreach(QPolygon poly, polyList){
        painter.drawPolygon(poly);
    }
    // render the holes;
    painter.setPen(QColor(0,0,0));
    foreach(QPolygon poly, holeList){
        painter.drawPolygon(poly, Qt::FillRule::WindingFill);
    }
    */
}

void Canvas::addToPolyList(QList<QPolygon> &list, QPolygon *other)
{
    for(auto poly = list.begin(); poly != list.end(); poly++){
        if(poly->intersects(*other)){

            *other = poly->united(*other);
            list.erase(poly);
            //return;
        }
    }
    list.append(*other);
}

const Grid Canvas::getGrid() const
{
    return Grid(grid);
}

void Canvas::writeJson(QJsonObject &json) const
{
    QList<QPolygonF> polys = shapes.toFillPolygons();
    QJsonArray jsonPolygons; // the array jsonPolygons will represent the QPainterPath

    for(auto poly = polys.begin(); poly != polys.end(); poly++){ // iterate through all the polygons
        QJsonArray jsonPoints; // the array jsonPoints will represent a polygon
        for(auto point = poly->begin(); point != poly->end(); point++){
            // store each point of the polygon into a jsonArray
            jsonPoints.append(QString(POINTFORMAT).arg(point->x()).arg(point->y()));
        }
        // store each polygon into a jsonArray
        jsonPolygons.append(jsonPoints);
    }
    json.insert(JSONPOLYGONKEY, jsonPolygons); // insert the array of polygons into the json
    // DELETEME debugging the correct formation of the json object!
}

void Canvas::readJson(const QJsonObject &json)
{
    // if the json is malformed, such that it does not contain an array under the "polygons" key, then do nothing
    if(!json.contains(JSONPOLYGONKEY) || !json[JSONPOLYGONKEY].isArray())
        return;

    QPainterPath aux = QPainterPath();
    QJsonArray jsonArray = json[JSONPOLYGONKEY].toArray();
    for(auto jsonPoly = jsonArray.begin(); jsonPoly != jsonArray.end(); jsonPoly++){ // iterate through polygons
        if(jsonPoly->isArray()){ // if current item in array is another array, it must be an array of points, thus, a polygon
            QJsonArray jsonPoints = jsonPoly->toArray();
            QVector<QPointF> pointList;
            for(auto jsonPoint = jsonPoints.begin(); jsonPoint != jsonPoints.end(); jsonPoint++){// iterate through points
                if(jsonPoint->isString()){
                    // split the string into 2 doubles
                    QRegExp regex(POINTSPLITREGEX);
                    QStringList splitted = jsonPoint->toString().split(regex, QString::SplitBehavior::SkipEmptyParts);
                    if(splitted.size() < 2) // if there is less than 2 doubles, it ain't no point pal!
                        throw std::runtime_error("error while parsing point: "+jsonPoint->toString().toUtf8()); // we'll panic
                    qreal x, y;
                    x = splitted[0].toDouble();
                    y = splitted[1].toDouble();
                    pointList.append(QPointF(x,y)); // create the point at last!
                }
            }
            // at the end of this loop, we should have all points of a polygon inside pointList
            aux.addPolygon(QPolygonF(pointList));
        }
    }
    //README do we have to free memory????
    shapes = aux;
}
