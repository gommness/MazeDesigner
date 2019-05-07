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

void KeyRepository::writeJson(QJsonObject &json)
{
    json.insert(JSONKEYMAXID, Key::ID);
    QJsonArray jsonArray;
    for(auto key = begin(); key != end(); key++){
        jsonArray.append(key->toJson());
    }
    json.insert(JSONKEYKEY, jsonArray);
}

void KeyRepository::readJson(const QJsonObject &json)
{
    KeyRepository aux;
    int keyID = -1;

    if(json.contains(JSONKEYMAXID) && json[JSONKEYMAXID].isDouble())
        keyID = json[JSONKEYMAXID].toInt();
    else
        throw std::runtime_error("no MAX-ID found in jsonObject");

    if(json.contains(JSONKEYKEY) && json[JSONKEYKEY].isArray()){
        QJsonArray jsonArray = json[JSONKEYKEY].toArray();
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
