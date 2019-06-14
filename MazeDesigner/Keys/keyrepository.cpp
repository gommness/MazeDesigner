#include "keyrepository.h"
#include <QJsonArray>

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

void KeyRepository::removeAt(int i)
{
    // notify that the i-th key is going to be deleted to whoever is connected
    emit keyDeleted((*this)[i]);
    QList::removeAt(i); // then actually delete the key
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

bool KeyRepository::contains(const QString & keyname) const
{
    for(auto key = begin(); key != end(); key++)
        if(key->getName() == keyname)
            return true;
    return false;
}

bool KeyRepository::contains(int ID) const
{
    for(auto key = begin(); key != end(); key++)
        if(key->getId() == ID)
            return true;
    return false;
}

const Key &KeyRepository::keyWithId(int ID) const
{
    for(auto key = begin(); key != end(); key++)
        if(key->getId() == ID)
            return *key;
    return Key::invalidKey();
}

const Key &KeyRepository::keyWithName(const QString &name) const
{
    for(auto key = begin(); key != end(); key++)
        if(key->getName() == name)
            return *key;
    return Key::invalidKey();
}

void KeyRepository::writeJson(QJsonObject &json)
{
    QJsonObject wrapper;
    wrapper.insert(JSONKEYMAXID, Key::ID);
    QJsonArray jsonArray;
    for(auto key = begin(); key != end(); key++){
        jsonArray.append(key->toJson());
    }
    wrapper.insert(JSONKEYKEY, jsonArray);
    json.insert(JSONKEYREPOKEY, wrapper);
}

void KeyRepository::readJson(const QJsonObject &json)
{
    KeyRepository aux;
    int keyID;
    QJsonObject innerJson;
    if(json.contains(JSONKEYREPOKEY) && json[JSONKEYREPOKEY].isObject())
        innerJson = json[JSONKEYREPOKEY].toObject();
    else
        throw std::runtime_error("no key-repository key found in jsonObject");
    if(innerJson.contains(JSONKEYMAXID) && innerJson[JSONKEYMAXID].isDouble())
        keyID = innerJson[JSONKEYMAXID].toInt();
    else
        throw std::runtime_error("no MAX-ID found in jsonObject");

    if(innerJson.contains(JSONKEYKEY) && innerJson[JSONKEYKEY].isArray()){
        QJsonArray jsonArray = innerJson[JSONKEYKEY].toArray();
        for(auto jsonKey = jsonArray.begin(); jsonKey != jsonArray.end(); jsonKey++){
            if(jsonKey->isObject()){
                Key key(jsonKey->toObject());
                aux.append(key);
            }
        }
    }

    Key::ID = keyID;
    clear();
    append(aux);
}

/*
void KeyRepository::onKeyChanged(const int &index, const QString &keyname, const bool &value)
{
    // for some reason, accessing the operator [] directly does not retrieve a Key. Must be because of multiple inheritance issues
    this->operator[](index).setName(keyname);
    this->operator[](index).setPowerUp(value);
}
*/
