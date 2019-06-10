#ifndef DOORCONDITIONCONSOLE_H
#define DOORCONDITIONCONSOLE_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include "../ConditionParser/compositecondition.h"
#include "doorinstance.h"

class DoorConditionConsole : public QWidget
{
    Q_OBJECT
public:
    DoorConditionConsole(QWidget * parent, KeyRepository * repo);
    ~DoorConditionConsole() override;
signals:
    void condition1Changed(SimpleCondition & cond);
    void condition2Changed(SimpleCondition & cond);

public slots:
    void onDoorSelected(DoorInstance & door);
    void onClearSelection();
private slots:
    void onSat1Changed();
    void onSat2Changed();
    void onText1Changed();
    void onText2Changed();
private:
    void connectSignals();
    void disconnectSignals();
    KeyRepository* repo = nullptr;
    const DoorInstance * selected = nullptr;
    QCheckBox * sat1;
    QLabel * label1;
    QTextEdit * condition1;
    QCheckBox * sat2;
    QLabel * label2;
    QTextEdit * condition2;
    QPushButton * submit;
};

#endif // DOORCONDITIONCONSOLE_H
