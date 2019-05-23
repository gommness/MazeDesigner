#ifndef ROOMLISTWIDGET_H
#define ROOMLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "../Canvas/roomcanvas.h"

class RoomListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomListWidget(QWidget *parent = nullptr, RoomCanvas * repo = nullptr);
    ~RoomListWidget();
    void load();
    void load(RoomCanvas * repo);
    const Room & selectedRoom();

signals:
    void cellChanged(const int & index, const QString & keyname, const bool& value);

public slots:
    void onCellChanged(int row, int column);
    void onRoomCreated(const Room & room);
    void onRoomSelected(const Room & room);
    void onRoomDestroyed(const Room & room);

private:
    QTableWidget *table = nullptr;
    RoomCanvas *repo;

    void connectSignals();
friend class RoomCanvas;
};

#endif // ROOMLISTWIDGET_H
