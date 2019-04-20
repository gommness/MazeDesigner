#include "keyeditorwidget.h"
#include "keylistwidget.h"
#include <QApplication>

int main(int argv, char **args)
{
    QApplication app(argv, args);

    KeyListWidget window;
    window.show();

    return app.exec();
}
