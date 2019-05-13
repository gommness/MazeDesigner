#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Keys/keylistwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextBlock>
#include <QStackedLayout>

#include <QDebug>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout * mainLayout = new QHBoxLayout;

    QVBoxLayout * editionLayout = new QVBoxLayout;
    // this piece will go in the graphical editor section of the application
    QTabWidget * designTabs = new QTabWidget;
    keyRepo = new KeyRepository;
    keyListWidget = new KeyListWidget(nullptr, keyRepo);
    designCanvas = new Canvas;
    roomCanvas = new RoomCanvas(designCanvas);
    instanceCanvas = new InstanceCanvas(designCanvas, keyListWidget);
    //QLabel *designRoomRegions = new QLabel("label on top numer 1!", designCanvas);
    //QLabel * designInstances = new QLabel("label on top number 2!", designCanvas);
    designTabs->addTab(designCanvas, "Canvas");
    designTabs->addTab(roomCanvas, "Rooms");
    designTabs->addTab(instanceCanvas, "Instances");
    /// README this has been tested and as a result, the widget classes that will have the canvas
    /// as their parent will have to actively update its parent when they themselves are updated
    /// this is no big deal since I had to code said classes anyways, but is nice to have
    /// sppoted this early on!!

    // this piece will go in the text display section of the application
    QTabWidget * textTabs = new QTabWidget;
    QTextEdit * textDoorEditor = new QTextEdit;
    QTextEdit * console = new QTextEdit;
    console->setReadOnly(true);
    textTabs->addTab(console, "console");
    textTabs->addTab(textDoorEditor, "doorInfo"); // we need to create a more sofisticated widget to display door info

    // piece both the canvases and texts sections together
    editionLayout->addWidget(designTabs,90);
    editionLayout->addWidget(textTabs, 10);

    // the stacked widget contains the widgets that display info in tables
    // we will have to, somehow connect this widget with the designTabs from before
    // probably by inheriting from the class or connecting some slots to some signals (?)
    QStackedLayout * leftDisplay = new QStackedLayout;
    QLabel * roomsTableDisplay = new QLabel("I'm a place-holder!");
    leftDisplay->addWidget(keyListWidget);
    leftDisplay->addWidget(roomsTableDisplay);

    //piece both layouts together into the whole application
    mainLayout->addLayout(leftDisplay,20);
    mainLayout->addLayout(editionLayout,80);


    // now onto the less obvios stuff, such as the top, and bottom fillers AND the top menus
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addLayout(mainLayout,2000);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);


    // then we create the menus
    createActions();
    createMenus();

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);

    //this->setLayout(&mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newDesign()
{

}

void MainWindow::openDesign()
{
    QJsonObject json;
    InstanceCanvas other(designCanvas, keyListWidget);
    instanceCanvas->writeJson(json);
    QString str(QJsonDocument(json).toJson(QJsonDocument::Indented));
    qDebug().noquote() << "original json: "<< str;
    other.readJson(json);
    QJsonObject json2;
    other.writeJson(json2);
    QString str2(QJsonDocument(json2).toJson(QJsonDocument::Indented));
    qDebug().noquote() << "new json: "<< str2;
    qDebug() << "are jsons equal?" << (str == str2);
}

void MainWindow::saveDesign()
{
    QJsonObject json;
    designCanvas->writeJson(json);
    roomCanvas->writeJson(json);
    keyRepo->writeJson(json);
    QString str(QJsonDocument(json).toJson(QJsonDocument::Indented));
    qDebug().noquote() << "json: "<< str;
}

void MainWindow::saveAsDesign()
{

}

void MainWindow::validateDesign()
{

}

void MainWindow::exportDesign()
{

}

void MainWindow::createActions()
{
    // new file action
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new design"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newDesign);
    // open file action
    openAct = new QAction(tr("&open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a design"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openDesign);
    // save action
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("saves the design"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveDesign);
    // save-as action
    saveAsAct = new QAction(tr("&Save as"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("saves the design"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsDesign);
    // validate action
    validateAct = new QAction(tr("&Validate"), this);
    //validateAct->setShortcuts(QList<QKeySequence>(new QKeySequence(Qt::Key_F5)));
    validateAct->setStatusTip(tr("Validates the current design"));
    connect(validateAct, &QAction::triggered, this, &MainWindow::validateDesign);
    // export action
    exportAct = new QAction(tr("&Export"), this);
    //exportAct->setShortcuts(QKeySequence::New);
    exportAct->setStatusTip(tr("Export current design into game maker rooms"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::exportDesign);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(validateAct);
    fileMenu->addAction(exportAct);
}
