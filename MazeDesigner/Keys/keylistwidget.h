#ifndef KEYLISTWIDGET_H
#define KEYLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "keyrepository.h"

/*
 * TODO:
 *      - el metodo para eliminar debe preguntar por confirmacion (?)
 */

class KeyListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyListWidget(QWidget *parent = nullptr, KeyRepository * repo = nullptr);
    ~KeyListWidget();
    void load();
    void load(KeyRepository * repo);

signals:
    void cellChanged(const int & index, const QString & keyname, const bool& value);

public slots:
    void createKey();
    void deleteKey();
    void onCellChanged(int row, int column);

private:
    QPushButton *createButton = nullptr;
    QPushButton *removeButton = nullptr;
    QTableWidget *table = nullptr;
    KeyRepository *repo;

    void connectSignals();
};

#endif // KEYLISTWIDGET_H
