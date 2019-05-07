#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include "../Canvas/canvas.h"
#include "../Canvas/roomcanvas.h"
#include "../Keys/keyrepository.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    void newDesign();
    void openDesign();
    void saveDesign();
    void saveAsDesign();
    void validateDesign();
    void exportDesign();

private:
    void createActions();
    void createMenus();
    QMenu * fileMenu;
    QAction * newAct;
    QAction * openAct;
    QAction * saveAct;
    QAction * saveAsAct;
    QAction * validateAct;
    QAction * exportAct;
    Ui::MainWindow *ui;

    RoomCanvas * roomCanvas = nullptr;
    Canvas * designCanvas = nullptr;
    KeyRepository * keyRepo = nullptr;
};

#endif // MAINWINDOW_H
