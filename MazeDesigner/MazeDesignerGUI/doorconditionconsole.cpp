#include "doorconditionconsole.h"
#include <QFormLayout>
#include <QResizeEvent>

DoorConditionConsole::DoorConditionConsole(QWidget *parent, KeyRepository *repo) : QWidget (parent), repo(repo) {
    QFormLayout * lay = new QFormLayout(parent);
    label1 = new QLabel("From A to B:",this);
    label2 = new QLabel("From B to A:",this);
    condition1 = new QTextEdit(this);
    condition2 = new QTextEdit(this);
    sat1 = new QCheckBox("satisfiable");
    sat2 = new QCheckBox("satisfiable");
    submit = new QPushButton("submit");
    lay->addWidget(sat1);
    lay->addRow(label1, condition1);
    lay->addWidget(sat2);
    lay->addRow(label2, condition2);
    lay->addWidget(submit);
    this->setLayout(lay);
    //connect(condition1, &QTextEdit::textChanged, this, &DoorConditionConsole::onText1Changed);
    //connect(condition2, &QTextEdit::textChanged, this, &DoorConditionConsole::onText2Changed);
}

DoorConditionConsole::~DoorConditionConsole()
{
    delete label1;
    delete label2;
    delete condition1;
    delete condition2;
    delete sat1;
    delete sat2;
    delete submit;
    delete layout();
}

void DoorConditionConsole::onDoorSelected(DoorInstance &door)
{
    onClearSelection();
    //firs we disconnect the signals to avoid a signal feedback loop
    //sat1->setCheckState(Qt::CheckState::Unchecked);
    //sat2->setCheckState(Qt::CheckState::Unchecked);
    if(door.condition1 != nullptr){
        condition1->setText(door.condition1->toString());
        if(door.condition1->isSatisfiable())
            sat1->setCheckState(Qt::CheckState::Checked);
        else
            sat1->setCheckState(Qt::CheckState::Unchecked);
    }
    if(door.condition2 != nullptr){
        condition2->setText(door.condition2->toString());
        if(door.condition2->isSatisfiable())
            sat2->setCheckState(Qt::CheckState::Checked);
        else
            sat2->setCheckState(Qt::CheckState::Unchecked);
    }
    selected = &door;
    connectSignals();
}

void DoorConditionConsole::onClearSelection()
{
    disconnectSignals();
    selected = nullptr;
    sat1->setCheckState(Qt::CheckState::Unchecked);
    sat2->setCheckState(Qt::CheckState::Unchecked);
    condition1->setText("");
    condition2->setText("");
}

void DoorConditionConsole::onSat1Changed()
{
    onText1Changed();
}

void DoorConditionConsole::onSat2Changed()
{
    onText2Changed();
}

void DoorConditionConsole::onText1Changed()
{
    SimpleCondition cond(condition1->toPlainText(), repo, sat1->isChecked());
    emit condition1Changed(cond);
}

void DoorConditionConsole::onText2Changed()
{
    SimpleCondition cond(condition2->toPlainText(), repo, sat2->isChecked());
    emit condition2Changed(cond);
}

void DoorConditionConsole::connectSignals()
{
    connect(submit, &QPushButton::clicked, this, &DoorConditionConsole::onText1Changed);
    connect(submit, &QPushButton::clicked, this, &DoorConditionConsole::onText2Changed);
    connect(sat1, &QCheckBox::clicked, this, &DoorConditionConsole::onSat1Changed);
    connect(sat2, &QCheckBox::clicked, this, &DoorConditionConsole::onSat2Changed);
    if(selected != nullptr){
        connect(this, &DoorConditionConsole::condition1Changed, selected, &DoorInstance::onCondition1Changed);
        connect(this, &DoorConditionConsole::condition2Changed, selected, &DoorInstance::onCondition2Changed);
    }
}

void DoorConditionConsole::disconnectSignals()
{
    disconnect(submit, &QPushButton::clicked, this, &DoorConditionConsole::onText1Changed);
    disconnect(submit, &QPushButton::clicked, this, &DoorConditionConsole::onText2Changed);
    disconnect(sat1, &QCheckBox::clicked, this, &DoorConditionConsole::onSat1Changed);
    disconnect(sat2, &QCheckBox::clicked, this, &DoorConditionConsole::onSat2Changed);
    if(selected != nullptr){
        disconnect(this, &DoorConditionConsole::condition1Changed, selected, &DoorInstance::onCondition1Changed);
        disconnect(this, &DoorConditionConsole::condition2Changed, selected, &DoorInstance::onCondition2Changed);
    }
}
