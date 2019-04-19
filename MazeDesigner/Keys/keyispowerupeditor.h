#ifndef KEYISPOWERUPEDITOR_H
#define KEYISPOWERUPEDITOR_H

#include <QCheckBox>

class KeyIsPowerUpEditor : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(Qt::CheckState isPowerUp READ isPowerUp WRITE setIsPowerUp USER true)
public:
    KeyIsPowerUpEditor(QWidget* widget = nullptr);

    Qt::CheckState isPowerUp() const;
    void setIsPowerUp(const Qt::CheckState & state);
};

#endif // KEYISPOWERUPEDITOR_H
