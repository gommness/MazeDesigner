#ifndef KEYNAMELISTEDITOR_H
#define KEYNAMELISTEDITOR_H

#include <QLineEdit>

class KeyNameEditor : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName USER true)

public:
    KeyNameEditor(QWidget* widget = nullptr);

    QString name() const;
    void setName(const QString & name);
};

#endif // KEYNAMELISTEDITOR_H
