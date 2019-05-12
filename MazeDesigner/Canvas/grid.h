#ifndef GRID_H
#define GRID_H
#include <QPoint>
#include <QWidget>
#include <stdint.h>

class Grid : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Grid creates a grid
     * @param parent
     */
    Grid(QWidget * parent);
    /**
     * @brief Grid creates a copy of a given grid
     * @param grid
     */
    Grid(const Grid & grid);
    /**
     * @brief nearestPoint calculates which point of the grid is the closest to the given point
     * @param other the point given to perform the calculation
     * @return the in-grid QPointF which is closets to the given point
     */
    QPointF nearestPoint(const QPointF& other) const;
    /**
     * @brief centerOfCellAt returns the center of the grid's cell that contains the given point
     * @param point the point that "selects" a cell
     * @return a point whose coordinates are the center of the "selected" cell
     */
    QPointF centerOfCellAt(const QPointF& point) const;
    /**
     * @brief adapted perform the pertinent grid's offset and scaling transformations to the given point
     * @param point the point to apply transformations to
     * @return the point with the transformations performed
     */
    QPointF adapted(const QPointF & point) const;
    /**
     * @brief getSize retrieves the current cell size of the grid. Not affected by scalling
     * @return the grid size
     */
    int8_t getSize() const;
    /**
     * @brief setSize sets the size of the grid
     * @param value the value to be set
     */
    void setSize(const int8_t &value);
    /**
     * @brief mouseMoveEventHandler handles the mouse moving input from the user. If the user pressed the middle mouse button
     * beforehand, then an offset transformation is performed to the grid
     * @param event
     */
    void mouseMoveEventHandler(QMouseEvent* event);
    /**
     * @brief mousePressEventHandler handles the mouse pressing input from the user. It will start a offset transformation if
     * the button pressed is the middle one
     * @param event
     */
    void mousePressEventHandler(QMouseEvent * event);
    /**
     * @brief mouseReleaseEventHandler handles the mouse pressing input from the user. It will terminate the offset transformation.
     * @param event
     */
    void mouseReleaseEventHandler(QMouseEvent * event);
    /**
     * @brief wheelEvent handles the scrolling input from the user, scaling up or down the grid
     * @param event
     */
    void wheelEvent(QWheelEvent *event) override;
    /**
     * @brief sizeHint
     * @return
     */
    QSize sizeHint() const override;
    /**
     * @brief getOffset retrieves the current offset as a vector stored in a QPoint object
     * @return current offset as a vector stored in a QPoint object
     */
    QPoint getOffset() const;
    /**
     * @brief getScale retrieves the current scale of the grid
     * @return the current scale
     */
    qreal getScale() const;
    /**
     * @brief pointDistance basic operationr that calculates the distance between two points
     * @param p1 the first point
     * @param p2 the second point
     * @return the distance between the two points
     */
    static double pointDistance(const QPointF& p1, const QPointF& p2);


protected:
    /**
     * @brief paintEvent draws the grid as vertical and horizontal lines
     */
    void paintEvent(QPaintEvent *) override;

private:
    /**
     * @brief updateOffset updates the offset by calculating the vector described with two points
     * @param from the point that is also the origin point of the movement vector
     * @param to the point that is the end of the movement vector
     */
    void updateOffset(const QPoint * from, const QPoint * to);

    int8_t size = 1;
    QPoint * previous = nullptr;
    QPoint * current = nullptr;
    qreal scale = 1;
    int hOffset = 0;
    int vOffset = 0;

};

#endif // GRID_H
