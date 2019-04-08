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

protected:
    void paintEvent(QPaintEvent *event);

private:
    static double pointDistance(const QPoint& p1, const QPoint& p2);
    int8_t size = 1;

};

#endif // GRID_H
