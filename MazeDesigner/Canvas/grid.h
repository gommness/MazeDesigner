#ifndef GRID_H
#define GRID_H
#include <QPoint>
#include <QWidget>
#include <stdint.h>

class Grid : public QWidget
{
    Q_OBJECT
public:
    Grid(QWidget * parent);
    Grid(const Grid & grid);
    QPointF nearestPoint(const QPointF& other, qreal gridOffsetted = 0) const;
    int8_t getSize() const;
    void setSize(const int8_t &value);
    void mouseMoveEventHandler(QMouseEvent* event);
    void mousePressEventHandler(QMouseEvent * event);
    void mouseReleaseEventHandler(QMouseEvent * event);
    void wheelEvent(QWheelEvent *event) override;
    QSize sizeHint() const override;
    QPoint getOffset() const;
    qreal getScale() const;
    static double pointDistance(const QPointF& p1, const QPointF& p2);


protected:
    void paintEvent(QPaintEvent *) override;

private:
    void updateOffset(const QPoint * from, const QPoint * to);
    int8_t size = 1;
    QPoint * previous = nullptr;
    QPoint * current = nullptr;
    qreal scale = 1;
    int hOffset = 0;
    int vOffset = 0;

};

#endif // GRID_H
