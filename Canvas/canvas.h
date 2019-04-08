#ifndef CANVAS_H
#define CANVAS_H

#include "grid.h"

#include <QWidget>
#include <QPen>
#include <QPaintEvent>
#include <QPainter>

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

public slots:
    void setPen(const QPen &pen);
    void setTransformed(bool transformed);

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief addPolygon adds a polygon to the list. if the new polygon intersects another polygon, the former polygon is transformed
     * into the union of itself and the new polygon
     * @param poly the new polygon to add to the list
     */
    void addPolygon(const QPolygon & poly);
    /**
     * @brief render to be called only within the paintEvent. Renders all the polygons stored
     */
    void render();

private:
    QList<QPolygon> polyList;
    QPainter painter;
    QPen pen;
    bool transformed;
    QPoint *start = nullptr;
    QPoint *end = nullptr;
    Grid grid;
};

#endif // CANVAS_H
