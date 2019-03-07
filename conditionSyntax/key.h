#ifndef KEY_H
#define KEY_H
#include <QString>

class Key
{
public:
    enum Type {UNDEFINED, NUMERIC, BOOLEAN};

/// members
public:
    Type type;
private:

/// methods
public:
    Key() : type{Type::UNDEFINED} {}
    Key(const Type &type) : type{type} {}
};

#endif // KEY_H
