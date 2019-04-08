#include <algorithm>
#include <QtWidgets>
#include "keylistwidget.h"
#include <QItemEditorFactory>
#include "keynameeditor.h"
#include <QTableWidget>

KeyListWidget::KeyListWidget(QWidget *parent) : QWidget(parent)
{
    QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *textListEditor =
        new QStandardItemEditorCreator<KeyNameEditor>();

    factory->registerEditor(QVariant::String, textListEditor);

    QItemEditorFactory::setDefaultFactory(factory);

    this->createButton = new QPushButton();
    this->createButton->setText("create key");
    this->removeButton = new QPushButton();
    this->removeButton->setText("remove key");

    connect(createButton, &QPushButton::clicked, this, &KeyListWidget::createKey);
    connect(removeButton, &QPushButton::clicked, this, &KeyListWidget::deleteKey);

    // this->repo << Key("key1") << Key("key2") << Key("key3"); // DELETEME
    this->table = new QTableWidget();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList() << tr("key name") << tr("is power up"));
    table->verticalHeader()->setVisible(false);
    table->resize(150, 50);
    KeyRepository rr = KeyRepository();
    rr << Key("key1") << Key("key2");
    this->load(rr);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(createButton, 0, 0);
    layout->addWidget(removeButton, 0, 1);
    layout->addWidget(table, 1, 0);
    setLayout(layout);
}

KeyListWidget::~KeyListWidget()
{
    delete createButton;
    delete removeButton;
    delete table;
}

void KeyListWidget::load()
{
    //table = new QTableWidget(repo.length(), 2);
    //table->resize(repo.length(), 2);

    table->setRowCount(repo.length());
    for (int i = 0; i < repo.length(); ++i) {
        //table->insertRow(table->rowCount()); // README this would work!! useful for update methods
        Key key = repo.at(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(key.getName());
        QTableWidgetItem *boolItem = new QTableWidgetItem;
        boolItem->setData(Qt::CheckStateRole, key.isPowerUp());

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, boolItem);
    }
    table->resizeColumnToContents(0);
    table->horizontalHeader()->setStretchLastSection(true);

    //setWindowTitle(tr("String Editor Factory"));
}

void KeyListWidget::load(const KeyRepository & repo)
{
    this->repo = repo;
    load();
}

void KeyListWidget::createKey()
{
    int rowCount = table->rowCount();
    table->insertRow(rowCount); // inserts new row at the bottom
    Key key("new key"); // create a new key
    repo << key; // insert into the repo

    QTableWidgetItem *nameItem = new QTableWidgetItem(key.getName());
    QTableWidgetItem *boolItem = new QTableWidgetItem;
    boolItem->setData(Qt::CheckStateRole, key.isPowerUp());
    // obtain representable key info and then update the table info with it!!
    table->setItem(rowCount, 0, nameItem);
    table->setItem(rowCount, 1, boolItem);
    table->selectRow(rowCount);
    // TODO connect key for data update if it was necessary (but I don't think so)
}

void KeyListWidget::deleteKey()
{
    // TODO eliminar las rows de la vista
    //      luego eliminar las llaves del repositorio de llaves
    QSet<int> indexesToRemove;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    for(int i = 0; i < ranges.length(); i++){
        QTableWidgetSelectionRange range = ranges[i];
        int bottomRow = range.bottomRow();
        int topRow = range.topRow();
        for(int j = bottomRow; j >= topRow; j--){
            indexesToRemove.insert(j);
        }
    }

    QList<int> removeList = indexesToRemove.toList();
    std::sort(removeList.begin(), removeList.end());
    for(int i = removeList.length()-1; i >= 0; i--)
        table->removeRow(removeList[i]);
}