#include "console.h"

Console::Console(QWidget *parent) : QTextEdit (parent) {
    this->setReadOnly(true);
}

void Console::onSelectInstance(const SelectableInstance &inst)
{
    this->setText(inst.instanceInfo());
}

void Console::onClearSelection()
{
    this->setText("");
}
