#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Keys/keylistwidget.h"
#include "../Common/common.h"
#include "exporter.h"
#include <QFileDialog>

#include <QDebug>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    TODO("testing");
    ui->setupUi(this);
    setUp();

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
    deleteDesign();
    setUp();
}

void MainWindow::openDesign()
{
    /// README ok so there is this weird bug that freezes the app if the currently selected tab is not the main design tab
    /// it probably has to be something with widget inheritance, so, in order to avoid it (I know it won't be pretty) we will
    /// set the current tab to 0, which will not freeze the app, save the file, then restore the current tab to what it was before
    /// I'm sorry but not really, this is kinda stupid
    int index = designTabs->tabBar()->currentIndex();
    newDesign();
    designTabs->tabBar()->setCurrentIndex(0);
    QString strFile = QFileDialog::getOpenFileName(this, tr("Open design"), "", tr("Maze Designs (*.maze);;All Files(*)"));
    if(strFile.isEmpty())
        return;
    //qDebug()<<strFile;
    QFile file(strFile);
    file.open(QIODevice::ReadOnly);
    QString jsonString = file.readAll();
    file.close();
    QJsonDocument jDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jObj = jDoc.object();
    designCanvas->readJson(jObj);
    roomCanvas->readJson(jObj);
    roomListWidget->load();
    keyRepo->readJson(jObj);
    keyListWidget->load();
    instanceCanvas->readJson(jObj);
    designTabs->tabBar()->setCurrentIndex(index);
    designCanvas->update(); // yeah wtf, otherwise, the canvas would not be updated
    /*
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
    */
}

void MainWindow::saveDesign()
{
    /// README ok so there is this weird bug that freezes the app if the currently selected tab is not the main design tab
    /// it probably has to be something with widget inheritance, so, in order to avoid it (I know it won't be pretty) we will
    /// set the current tab to 0, which will not freeze the app, save the file, then restore the current tab to what it was before
    /// I'm sorry but not really, this is kinda stupid
    int index = designTabs->tabBar()->currentIndex();
    designTabs->tabBar()->setCurrentIndex(0);
    if(filename.isEmpty())
        filename = QFileDialog::getSaveFileName(this, tr("Save maze design"),".maze",tr("Maze Designs (*.maze);;All Files(*)"));
    saveFile(filename);
    designTabs->tabBar()->setCurrentIndex(index);
    /*
    QJsonObject json;
    designCanvas->writeJson(json);
    roomCanvas->writeJson(json);
    keyRepo->writeJson(json);
    QString str(QJsonDocument(json).toJson(QJsonDocument::Indented));
    qDebug().noquote() << "json: "<< str;
    */
}

void MainWindow::saveAsDesign()
{
    /// README ok so there is this weird bug that freezes the app if the currently selected tab is not the main design tab
    /// it probably has to be something with widget inheritance, so, in order to avoid it (I know it won't be pretty) we will
    /// set the current tab to 0, which will not freeze the app, save the file, then restore the current tab to what it was before
    /// I'm sorry but not really, this is kinda stupid
    int index = designTabs->tabBar()->currentIndex();
    designTabs->tabBar()->setCurrentIndex(0);
    filename = QFileDialog::getSaveFileName(this, tr("Save maze design"),".maze",tr("Maze Designs (*.maze);;All Files(*)"));
    saveFile(filename);
    designTabs->tabBar()->setCurrentIndex(index);
}

void MainWindow::validateDesign()
{
}

void MainWindow::exportDesign()
{
    /// README ok so there is this weird bug that freezes the app if the currently selected tab is not the main design tab
    /// it probably has to be something with widget inheritance, so, in order to avoid it (I know it won't be pretty) we will
    /// set the current tab to 0, which will not freeze the app, save the file, then restore the current tab to what it was before
    /// I'm sorry but not really, this is kinda stupid
    int index = designTabs->tabBar()->currentIndex();
    designTabs->tabBar()->setCurrentIndex(0);
    QString filename = QFileDialog::getOpenFileName(this, tr("Select project"), "", tr("GameMaker Studio 2 project(*.yyp);;All Files(*)"));
    Exporter::exportDesign(filename, *designCanvas, *roomCanvas);
    designTabs->tabBar()->setCurrentIndex(index);
    designCanvas->update();
}

void MainWindow::selectDesignTab(int index)
{
    if(index == 1)
        leftDisplay->setCurrentIndex(1);
    else
        leftDisplay->setCurrentIndex(0);
}

void MainWindow::saveFile(const QString &str)
{
    if(str.isEmpty())
        return;
    QFile file(str);
    file.open(QIODevice::WriteOnly);
    QJsonObject json;
    designCanvas->writeJson(json);
    roomCanvas->writeJson(json);
    keyRepo->writeJson(json);
    instanceCanvas->writeJson(json);
    file.write(QJsonDocument(json).toJson());
    file.close();
}

void MainWindow::deleteDesign()
{
    /// aw shit, here we go again
    delete mainLayout;
    // delete editionLayout;
    delete textTabs;
    // delete textDoorEditor;
    // delete console;
    // delete leftDisplay;
    delete roomsTableDisplay;
    delete widget;
    // delete topFiller;
    // delete bottomFiller;
    // delete layout;

    // delete designTabs;
    // delete keyListWidget;
    // delete roomCanvas;
    // delete instanceCanvas;
    // delete designCanvas;
    delete keyRepo;

    mainLayout = nullptr;
    editionLayout = nullptr;
    textTabs = nullptr;
    textDoorEditor = nullptr;
    console = nullptr;
    leftDisplay = nullptr;
    roomsTableDisplay = nullptr;
    widget = nullptr;
    topFiller = nullptr;
    bottomFiller = nullptr;
    layout = nullptr;

    designTabs = nullptr;
    keyListWidget = nullptr;
    roomListWidget = nullptr;
    roomCanvas = nullptr;
    instanceCanvas = nullptr;
    designCanvas = nullptr;
    keyRepo = nullptr;
}

void MainWindow::setUp()
{
    mainLayout = new QHBoxLayout;

    editionLayout = new QVBoxLayout;
    // this piece will go in the graphical editor section of the application
    designTabs = new QTabWidget;
    keyRepo = new KeyRepository;
    keyListWidget = new KeyListWidget(nullptr, keyRepo);
    designCanvas = new Canvas;
    roomCanvas = new RoomCanvas(designCanvas);
    roomListWidget = new RoomListWidget(nullptr, roomCanvas);
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
    textTabs = new QTabWidget;
    textDoorEditor = new QTextEdit;
    console = new QTextEdit;
    console->setReadOnly(true);
    textTabs->addTab(console, "console");
    textTabs->addTab(textDoorEditor, "doorInfo"); // we need to create a more sofisticated widget to display door info

    // piece both the canvases and texts sections together
    editionLayout->addWidget(designTabs,90);
    editionLayout->addWidget(textTabs, 10);

    // the stacked widget contains the widgets that display info in tables
    // we will have to, somehow connect this widget with the designTabs from before
    // probably by inheriting from the class or connecting some slots to some signals (?)
    leftDisplay = new QStackedLayout;
    leftDisplay->addWidget(keyListWidget);
    leftDisplay->addWidget(roomListWidget);

    //piece both layouts together into the whole application
    mainLayout->addLayout(leftDisplay,20);
    mainLayout->addLayout(editionLayout,80);


    // now onto the less obvios stuff, such as the top, and bottom fillers AND the top menus
    widget = new QWidget;
    setCentralWidget(widget);

    topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addLayout(mainLayout,2000);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);

    connectSignals();
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

void MainWindow::connectSignals()
{
    connect(designTabs, &QTabWidget::currentChanged, this, &MainWindow::selectDesignTab);
}
