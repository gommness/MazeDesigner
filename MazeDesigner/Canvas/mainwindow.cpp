#include "canvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = new Canvas(parent);
    this->layout()->addWidget(canvas);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}
