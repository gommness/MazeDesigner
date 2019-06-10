#ifndef CONSOLE_H
#define CONSOLE_H

#include <QTextEdit>
#include "instancecanvas.h"

class Console : public QTextEdit
{
    Q_OBJECT
public:
    Console(QWidget * parent);
public slots:
    void onSelectInstance(const SelectableInstance & inst);
    void onClearSelection();
};

#endif // CONSOLE_H
