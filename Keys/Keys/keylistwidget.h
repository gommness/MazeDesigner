#ifndef KEYLISTWIDGET_H
#define KEYLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "keyrepository.h"

/*
 * TODO:
 *      1. metodos para añadir y eliminar una llave de la lista
 *      2. el metodo para eliminar debe preguntar por confirmacion (?)
 *      3. conexion de las acciones de la vista con los datos del repositorio
 *      4.
 */

class KeyListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyListWidget(QWidget *parent = nullptr);
    ~KeyListWidget();
    void load();
    void load(const KeyRepository & repo);

signals:

public slots:
    void createKey();
    void deleteKey();

private:
    QPushButton *createButton = nullptr;
    QPushButton *removeButton = nullptr;
    QTableWidget *table = nullptr;
    KeyRepository repo;
};

#endif // KEYLISTWIDGET_H
