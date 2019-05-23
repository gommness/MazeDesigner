#include "roomlistwidget.h"
#include <QItemEditorFactory>
#include <QHeaderView>
#include <QLayout>
#include <QLineEdit>

RoomListWidget::RoomListWidget(QWidget *parent, RoomCanvas *repo) : QWidget (parent), repo(repo)
{
    QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *textListEditor =
        new QStandardItemEditorCreator<QLineEdit>();

    factory->registerEditor(QVariant::String, textListEditor);

    QItemEditorFactory::setDefaultFactory(factory);

    // this->repo << Key("key1") << Key("key2") << Key("key3"); // DELETEME
    this->table = new QTableWidget();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList() << tr("Room name") << tr("export"));
    table->verticalHeader()->setVisible(false);
    table->resize(150, 50);
    this->setLayout(new QGridLayout());
    this->layout()->addWidget(table);
    //KeyRepository rr;
    //rr << Key("key1") << Key("key2");
    this->load();

    connectSignals();
}

RoomListWidget::~RoomListWidget()
{
    delete table;
}

void RoomListWidget::load()
{
    //table = new QTableWidget(repo.length(), 2);
    //table->resize(repo.length(), 2);
    if(repo == nullptr)
        return;
    table->setRowCount(repo->getRooms().length());
    for (int i = 0; i < repo->getRooms().length(); ++i) {
        Room room = repo->getRooms().at(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(room.getName());
        QTableWidgetItem *boolItem = new QTableWidgetItem;
        boolItem->setData(Qt::CheckStateRole, room.isExportable());

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, boolItem);
    }
    table->resizeColumnToContents(0);
    table->horizontalHeader()->setStretchLastSection(true);

    //setWindowTitle(tr("String Editor Factory"));
}

void RoomListWidget::load(RoomCanvas *repo)
{
    this->repo = repo;
    load();
}

const Room &RoomListWidget::selectedRoom()
{
    QList<QTableWidgetItem*> list = table->selectedItems();
    if(repo == nullptr || list.size() == 0)
        return Room::invalidRoom();
    QTableWidgetItem * item = list[0];
    return repo->getRooms().at(item->row());
}

void RoomListWidget::onCellChanged(int row, int column)
{
    // we use row as index, thus consistency between the view's indexes and model's indexes
    // emit cellChanged(row, keyname, value);
    if(column == 0){
        QString keyname = table->item(row, 0)->data(Qt::DisplayRole).toString();
        repo->getRooms()[row].setName(keyname);
    } else {
        bool value = table->item(row, 1)->data(Qt::CheckStateRole).toBool();
        repo->getRooms()[row].setExportable(value);
    }
}

void RoomListWidget::onRoomCreated(const Room &room)
{
    if(repo == nullptr)
        return;
    int rowCount = table->rowCount();
    table->insertRow(rowCount); // inserts new row at the bottom

    QTableWidgetItem *nameItem = new QTableWidgetItem(room.getName());
    QTableWidgetItem *boolItem = new QTableWidgetItem;
    boolItem->setData(Qt::CheckStateRole, room.isExportable());
    // obtain representable key info and then update the table info with it!!
    table->setItem(rowCount, 0, nameItem);
    table->setItem(rowCount, 1, boolItem);
    table->selectRow(rowCount);
    table->resizeColumnToContents(0);
}

void RoomListWidget::onRoomSelected(const Room &room)
{
    if(!room.isValid()){
        table->unsetCursor();
        return;
    }
    for(int i = 0; i < table->rowCount(); i++)
        if(table->item(i, 0)->text() == room.getName()){
            table->setCurrentItem(table->item(i,0));
            return;
        }
    table->unsetCursor();
}

void RoomListWidget::onRoomDestroyed(const Room &room)
{
    if(repo == nullptr)
        return;
    for(int i = 0; i < table->rowCount(); i++)
        if(table->item(i, 0)->text() == room.getName()){
            table->removeRow(i);
            break;
        }
    table->unsetCursor();
}

void RoomListWidget::connectSignals()
{
    connect(table, &QTableWidget::cellChanged, this, &RoomListWidget::onCellChanged);
    connect(repo, &RoomCanvas::roomCreated, this, &RoomListWidget::onRoomCreated);
    connect(repo, &RoomCanvas::roomDestroyed, this, &RoomListWidget::onRoomDestroyed);
    connect(repo, &RoomCanvas::roomSelected, this, &RoomListWidget::onRoomSelected);
}
