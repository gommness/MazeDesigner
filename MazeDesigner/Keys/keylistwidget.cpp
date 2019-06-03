#include <algorithm>
#include <QtWidgets>
#include "keylistwidget.h"
#include <QItemEditorFactory>
#include "keynameeditor.h"
#include <QTableWidget>

KeyListWidget::KeyListWidget(QWidget *parent, KeyRepository * repo) : QWidget(parent), repo(repo)
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

    this->table = new QTableWidget();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList() << tr("key name") << tr("is power up"));
    table->verticalHeader()->setVisible(false);
    table->resize(150, 50);
    this->load();

    QGridLayout *buttonsLayout = new QGridLayout;
    QGridLayout *layout = new QGridLayout;

    connectSignals();

    buttonsLayout->addWidget(createButton, 0, 0);
    buttonsLayout->addWidget(removeButton, 0, 1);
    layout->addLayout(buttonsLayout, 0, 0);
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
    if(repo == nullptr)
        return;
    table->setRowCount(repo->length());
    for (int i = 0; i < repo->length(); ++i) {
        //table->insertRow(table->rowCount()); // README this would work!! useful for update methods
        Key key = repo->at(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(key.getName());
        QTableWidgetItem *boolItem = new QTableWidgetItem;
        if(key.isPowerUp())
            boolItem->setData(Qt::CheckStateRole, Qt::CheckState::Checked);
        else
            boolItem->setData(Qt::CheckStateRole, Qt::CheckState::Unchecked);
        //boolItem->setData(Qt::CheckStateRole, key.isPowerUp());

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, boolItem);
    }
    table->resizeColumnToContents(0);
    table->horizontalHeader()->setStretchLastSection(true);

    //setWindowTitle(tr("String Editor Factory"));
}

void KeyListWidget::load(KeyRepository * repo)
{
    this->repo = repo;
    load();
}

const Key & KeyListWidget::selectedKey()
{
    QList<QTableWidgetItem*> list = table->selectedItems();
    if(repo == nullptr || list.size() == 0)
        return Key::invalidKey();
    QTableWidgetItem * item = list[0];
    return repo->at(item->row());
}

void KeyListWidget::createKey()
{
    if(repo == nullptr)
        return;
    int rowCount = table->rowCount();
    table->insertRow(rowCount); // inserts new row at the bottom
    Key key; // create a new key
    *repo << key; // insert into the repo

    QTableWidgetItem *nameItem = new QTableWidgetItem(key.getName());
    QTableWidgetItem *boolItem = new QTableWidgetItem;
    if(key.isPowerUp())
        boolItem->setData(Qt::CheckStateRole, Qt::CheckState::Checked);
    else
        boolItem->setData(Qt::CheckStateRole, Qt::CheckState::Unchecked);
    // obtain representable key info and then update the table info with it!!
    table->setItem(rowCount, 0, nameItem);
    table->setItem(rowCount, 1, boolItem);
    table->selectRow(rowCount);
    table->resizeColumnToContents(0);
}

void KeyListWidget::deleteKey()
{
    if(repo == nullptr)
        return;
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
    for(int i = removeList.length()-1; i >= 0; i--){
        table->removeRow(removeList[i]);
        repo->removeAt(removeList[i]);
    }
}

void KeyListWidget::onCellChanged(int row, int column)
{
    // we use row as index, thus consistency between the view's indexes and model's indexes
    // emit cellChanged(row, keyname, value);
    if(column == 0){
        QString keyname = table->item(row, 0)->data(Qt::DisplayRole).toString();
        (*repo)[row].setName(keyname);
    } else {
        bool value = table->item(row, 1)->data(Qt::CheckStateRole).toBool();
        (*repo)[row].setPowerUp(value);
    }
}

void KeyListWidget::connectSignals()
{
    connect(createButton, &QPushButton::clicked, this, &KeyListWidget::createKey);
    connect(removeButton, &QPushButton::clicked, this, &KeyListWidget::deleteKey);
    connect(table, &QTableWidget::cellChanged, this, &KeyListWidget::onCellChanged);
    //connect(this, &KeyListWidget::cellChanged, &repo, &KeyRepository::onKeyChanged);
}
