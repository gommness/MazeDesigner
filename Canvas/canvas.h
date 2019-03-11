#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPen>
#include <QPaintEvent>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    ~Canvas() override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    //void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setTransformed(bool transformed);

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPen pen;
    bool transformed;
    QPoint *start = nullptr;
    QPoint *end = nullptr;
};

#endif // CANVAS_H
