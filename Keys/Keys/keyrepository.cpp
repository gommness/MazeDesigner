#include "keyrepository.h"

KeyRepository &operator<<(KeyRepository &repo, const Key &val)
{
    repo.append(val);
    return repo;
}

/**
 * @brief KeyRepository::operator = is a copy assignment. Copies the other's list elements into self
 * @param other the other list to copy
 * @return this but with other's data copied into it
 */
KeyRepository::KeyRepository()
{

}

KeyRepository &KeyRepository::operator=(const KeyRepository &other)
{
    this->clear();
    for(int i = 0; i < other.length(); i++)
        this->append(other[i]);
    return *this;
}

QString KeyRepository::toString()
{
    QString output;
    for(int i = 0; i < length(); i++)
        output += this->operator[](i).toString() + " | ";
    return output;
}

/*
void KeyRepository::onKeyChanged(const int &index, const QString &keyname, const bool &value)
{
    // for some reason, accessing the operator [] directly does not retrieve a Key. Must be because of multiple inheritance issues
    this->operator[](index).setName(keyname);
    this->operator[](index).setPowerUp(value);
}
*/
