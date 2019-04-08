#include "key.h"

Key::Key(const QString &name, const bool &value)
{
    this->name = name;
    this->powerUp = value;
}

QString Key::getName() const
{
    return name;
}

void Key::setName(const QString &name)
{
    this->name = name;
}

bool Key::isPowerUp() const
{
    return powerUp;
}

void Key::setPowerUp(const bool &value)
{
    this->powerUp = value;
}
