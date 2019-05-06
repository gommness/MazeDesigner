#ifndef CANVAS_H
#define CANVAS_H

#include "grid.h"

#include <QWidget>
#include <QPen>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollArea>
#include <QJsonObject>

class Canvas : public QWidget
{
    Q_OBJECT
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

    const Grid getGrid() const;

    void toJson(QJsonObject & json) const;
    void fromJson(const QJsonObject & json);

public slots:
    void setPen(const QPen &pen);
    void setTransformed(bool transformed);

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief addPolygon adds a polygon to the list. if the new polygon intersects another polygon, the former polygon is transformed
     * into the union of itself and the new polygon
     * @param poly the new polygon to add to the list
     */
    void addPolygon(QPolygonF * other);
    void addHole(QPolygonF * other);

    void removePolygon(QPolygonF *poly);
    /**
     * @brief render to be called only within the paintEvent. Renders all the polygons stored
     */
    void render();

private:
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

};

#endif // CANVAS_H
