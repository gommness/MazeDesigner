#include <QtWidgets>

#include "mainwindow.h"
#include "keynameeditor.h"
#include "keyispowerupeditor.h"

MainWindow::MainWindow()
{
    QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *textListEditor =
        new QStandardItemEditorCreator<KeyNameEditor>();
    QItemEditorCreatorBase *checkBoxEditor =
        new QStandardItemEditorCreator<KeyIsPowerUpEditor>();

    factory->registerEditor(QVariant::String, textListEditor);
    factory->registerEditor(QVariant::Bool, checkBoxEditor);

    QItemEditorFactory::setDefaultFactory(factory);

    createGUI();
}

void MainWindow::createGUI()
{
    QList<QPair<QString, bool>> list;
    list << QPair<QString, bool>(QString("key1"), false) << QPair<QString, bool>(QString("key2"), true)
            << QPair<QString, bool>(QString("key3"), false);

    QTableWidget *table = new QTableWidget(3, 2);
    table->setHorizontalHeaderLabels(QStringList() << tr("key name") << tr("is power up"));
    table->verticalHeader()->setVisible(false);
    table->resize(150, 50);

    for (int i = 0; i < 3; ++i) {
        QPair<QString, bool> pair = list.at(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(pair.first);
        QTableWidgetItem *boolItem = new QTableWidgetItem;
        boolItem->setData(Qt::CheckStateRole, pair.second);

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, boolItem);
    }
    table->resizeColumnToContents(0);
    table->horizontalHeader()->setStretchLastSection(true);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table, 0, 0);

    setLayout(layout);

    setWindowTitle(tr("String Editor Factory"));
}
