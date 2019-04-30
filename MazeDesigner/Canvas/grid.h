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
    QPoint nearestPoint(const QPoint& other) const;
    int8_t getSize() const;
    void setSize(const int8_t &value);
    void mouseMoveEventHandler(QMouseEvent* event);
    void mousePressEventHandler(QMouseEvent * event);
    void mouseReleaseEventHandler(QMouseEvent * event);


protected:
    void paintEvent(QPaintEvent *);

private:
    static double pointDistance(const QPoint& p1, const QPoint& p2);
    void updateOffset(const QPoint * from, const QPoint * to);
    int8_t size = 1;
    QPoint * previous = nullptr;
    QPoint * current = nullptr;
    int64_t hOffset = 0;
    int64_t vOffset = 0;

};

#endif // GRID_H
