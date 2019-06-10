#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextBlock>
#include <QStackedLayout>
#include "../Canvas/canvas.h"
#include "../Canvas/roomcanvas.h"
#include "instancecanvas.h"
#include "roomlistwidget.h"
#include "console.h"
#include "doorconditionconsole.h"
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

    void selectDesignTab(int index);
    void selectConditionTab();

private:
    void saveFile(const QString & str);
    void deleteDesign();
    void setUp();
    void createActions();
    void createMenus();
    void connectSignals();
    QMenu * fileMenu;
    QAction * newAct;
    QAction * openAct;
    QAction * saveAct;
    QAction * saveAsAct;
    QAction * validateAct;
    QAction * exportAct;
    Ui::MainWindow *ui;

    QString filename;

    QHBoxLayout * mainLayout = nullptr;
    QVBoxLayout * editionLayout = nullptr;
    QTabWidget * textTabs = nullptr;
    DoorConditionConsole * textDoorEditor = nullptr;
    Console * console = nullptr;
    QStackedLayout * leftDisplay = nullptr;
    QLabel * roomsTableDisplay = nullptr;
    QWidget *widget = nullptr;
    QWidget *topFiller = nullptr;
    QWidget *bottomFiller = nullptr;
    QVBoxLayout *layout = nullptr;

    QTabWidget * designTabs = nullptr;
    KeyListWidget * keyListWidget = nullptr;
    RoomListWidget * roomListWidget = nullptr;
    RoomCanvas * roomCanvas = nullptr;
    InstanceCanvas * instanceCanvas = nullptr;
    Canvas * designCanvas = nullptr;
    KeyRepository * keyRepo = nullptr;
};

#endif // MAINWINDOW_H
