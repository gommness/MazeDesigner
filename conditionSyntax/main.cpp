#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtTest/QTest>
#include "condition.h"
#include "key.h"

#define VERIFY(a) fprintf(stdout, "test of line %d yields: %d\n", __LINE__, a)

void test() {
    Key key_1(Key::Type::NUMERIC);
    Key key_2(Key::Type::BOOLEAN);
    QMap<QString, Key> nameSpace;
    nameSpace.insert("key_1", key_1);
    nameSpace.insert("key_2", key_2);
    VERIFY(false);
    try {
        Condition("");
    } catch (ErrorMalformedCondition e) {
        VERIFY(true);
    }
    try {
        Condition("key_1");
    } catch (ErrorMalformedCondition e) {
        VERIFY(true);
    }
    try {
        Condition("key_1 ==");
    } catch (ErrorMalformedCondition e) {
        VERIFY(true);
    }
    try {
        Condition("key_1 == ");
    } catch (ErrorMalformedCondition e) {
        VERIFY(true);
    }
    try {
        Condition("Key_1 == 4 aaa");
    } catch (ErrorMalformedCondition e) {
        VERIFY(true);
    }

    VERIFY(Condition("key_1 == 1", &nameSpace).validate());
    VERIFY(Condition("key_1 == -1", &nameSpace).validate());
    VERIFY(!Condition("key_1 == aaa", &nameSpace).validate());
    VERIFY(!Condition("key_1 == true", &nameSpace).validate());

    VERIFY(Condition("key_2 == true", &nameSpace).validate());
    VERIFY(Condition("key_2 == false", &nameSpace).validate());
    VERIFY(!Condition("key_2 == 0", &nameSpace).validate());
    VERIFY(!Condition("key_2 == aaaa", &nameSpace).validate());

    VERIFY(!Condition("key_3 == true", &nameSpace).validate());

}

int main()
{
    test();
    return 0;
}
