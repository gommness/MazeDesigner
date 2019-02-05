#include "textfinder.h"
#include "ui_textfinder.h"
#include <QFile>
#include <QTextStream>

TextFinder::TextFinder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextFinder)
{
    ui->setupUi(this);
    loadTextFile();
}

TextFinder::~TextFinder()
{
    delete ui;
}

void TextFinder::on_findButton_clicked()
{
    QString targetText = ui->lineEdit->text();
    ui->textEdit_2->find(targetText, QTextDocument::FindFlag::FindWholeWords);
}

void TextFinder::loadTextFile(){
    QFile inputFile(":/textsrc.txt");
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui->textEdit_2->setPlainText(line);
    QTextCursor cursor = ui->textEdit_2->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    /*
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString text = stream.readAll();
    file.close();

    ui->textEdit_2->setPlainText(text);
    QTextCursor cursor = ui->textEdit_2->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    */
}
