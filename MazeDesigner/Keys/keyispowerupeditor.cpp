#include "keyispowerupeditor.h"
#include <QDebug>

KeyIsPowerUpEditor::KeyIsPowerUpEditor(QWidget *widget)
{
    this->setParent(widget);
    this->setCheckable(true);
    this->setIsPowerUp(Qt::Unchecked);
}

Qt::CheckState KeyIsPowerUpEditor::isPowerUp() const
{
    return this->checkState();
}

void KeyIsPowerUpEditor::setIsPowerUp(const Qt::CheckState &state)
{
    this->setCheckState(state);
    qDebug() << "checked!! state: " << state;
}
