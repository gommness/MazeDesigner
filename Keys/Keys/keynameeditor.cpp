#include "keynameeditor.h"
#include <QDebug>

KeyNameEditor::KeyNameEditor(QWidget *widget)
{
    this->setParent(widget);
    this->setText("");
}

QString KeyNameEditor::name() const
{
    return this->text();
}

void KeyNameEditor::setName(const QString & name)
{
    //qDebug() << "setName called. name: " << name;
    this->setText(name);
}
