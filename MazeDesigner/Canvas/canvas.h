#ifndef CANVAS_H
#define CANVAS_H

#include "grid.h"

#include <QWidget>
#include <QPen>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollArea>
#include <QJsonObject>
#include "../Common/common.h"

#define JSONPOLYGONKEY "polygons"

class Canvas : public QWidget
{
    Q_OBJECT
signals:
    void destroySpace(const QPolygonF & destroyed);
public:
    /**
     * @brief Canvas constructor for the class. if parent is null, it creates a new window. Otherwise, the object constructed
     * will be physically bounded by its parent. see @ref QWidget()
     * @param parent the widget to be parent for the canvas
     */
    Canvas(QWidget *parent = nullptr);
    ~Canvas() override;
    /**
     * @brief minimumSizeHint returns the minimum size supported by this widget
     * @return the minimum size supported by this widget
     */
    QSize minimumSizeHint() const override;
    /**
     * @brief sizeHint returns the size recommended for this widget
     * @return the size recommended for this widget
     */
    QSize sizeHint() const override;
    /**
     * @brief toString returns the object in a QString readable format
     * @return the object in a QString readable format
     */
    QString toString() const;
    /**
     * @brief getGrid returns the non-modifiable grid
     * @return the grid
     */
    const Grid getGrid() const;
    /**
     * @brief contains checks whether a given point is inside the design or not
     * @param point the point to be checked
     * @return true if the point is inside the design, AKA, the playable space
     */
    bool contains(const QPointF & point) const;
    /**
     * @brief writeJson writes its own information into the given json object, essentially serializing itself
     * @param json the json to write into
     */
    void writeJson(QJsonObject & json) const;
    /**
     * @brief readJson initializes its information from a canvas representation in a json object
     * @param json th+operation()e json object to parse
     */
    void readJson(const QJsonObject & json);

    QPainterPath getDesign() const;

public slots:
    /**
     * @brief setPen sets the pen information
     * @param pen the pen to set
     * @note this funcition probably has no use
     */
    void setPen(const QPen &pen);
    /**
     * @brief setTransformed sets whether the object is transformed or not
     * @param transformed the transformed value
     * @note this funcition probably has no use
     */
    void setTransformed(bool transformed);

protected:
    /**
     * @brief paintEvent draws all the design as well as the underlying grid
     * @param event
     */
    void paintEvent(QPaintEvent * event) override;
    /**
     * @brief mousePressEvent handles the pressing mouse input from the user. It is the start of an action, which can be
     * creating more playable area with left click or destroying playable area with right click
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent handles the moving mouse input from the user.
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent handles the releasing mouse input from the player. ending the action that had been buffered
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief addPolygon adds a polygon to the list. if the new polygon intersects another polygon, the former polygon is transformed
     * into the union of itself and the new polygon
     * @param poly the new polygon to add to the list
     */
    void addPolygon(QPolygonF * other);
    /**
     * @brief addHole adds a hole into the design
     * @param other
     * @note this funcition probably has no use
     */
    void addHole(QPolygonF * other);
    /**
     * @brief removePolygon removes a polygon from the design
     * @param poly the polygon to remove
     */
    void removePolygon(QPolygonF *poly);
    /**
     * @brief render to be called only within the paintEvent. Renders all the polygons stored
     */
    void render();

private:
    /**
     * @brief addToPolyList adds a polygon to the list of polygons
     * @param list
     * @param other
     * @note this funcition probably has no use
     */
    void addToPolyList(QList<QPolygon> & list, QPolygon * other);

    //QList<QPolygon> polyList;
    //QList<QPolygon> holeList;
    //QScrollArea *scrollArea;
    QPainterPath shapes;
    QPainter painter;
    QPen pen;
    bool transformed;
    QPointF *start = nullptr;
    QPointF *end = nullptr;
    Grid grid;


friend class RoomCanvas;
friend class InstanceCanvas;
friend class Exporter;
friend class DesignGraph;
};

#endif // CANVAS_H
