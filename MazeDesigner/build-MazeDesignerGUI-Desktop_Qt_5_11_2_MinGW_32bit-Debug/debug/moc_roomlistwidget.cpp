/****************************************************************************
** Meta object code from reading C++ file 'roomlistwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MazeDesignerGUI/roomlistwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'roomlistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RoomListWidget_t {
    QByteArrayData data[14];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RoomListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RoomListWidget_t qt_meta_stringdata_RoomListWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RoomListWidget"
QT_MOC_LITERAL(1, 15, 11), // "cellChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "index"
QT_MOC_LITERAL(4, 34, 7), // "keyname"
QT_MOC_LITERAL(5, 42, 5), // "value"
QT_MOC_LITERAL(6, 48, 13), // "onCellChanged"
QT_MOC_LITERAL(7, 62, 3), // "row"
QT_MOC_LITERAL(8, 66, 6), // "column"
QT_MOC_LITERAL(9, 73, 13), // "onRoomCreated"
QT_MOC_LITERAL(10, 87, 4), // "Room"
QT_MOC_LITERAL(11, 92, 4), // "room"
QT_MOC_LITERAL(12, 97, 14), // "onRoomSelected"
QT_MOC_LITERAL(13, 112, 15) // "onRoomDestroyed"

    },
    "RoomListWidget\0cellChanged\0\0index\0"
    "keyname\0value\0onCellChanged\0row\0column\0"
    "onRoomCreated\0Room\0room\0onRoomSelected\0"
    "onRoomDestroyed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RoomListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   46,    2, 0x0a /* Public */,
       9,    1,   51,    2, 0x0a /* Public */,
      12,    1,   54,    2, 0x0a /* Public */,
      13,    1,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Bool,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void RoomListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RoomListWidget *_t = static_cast<RoomListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cellChanged((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 1: _t->onCellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onRoomCreated((*reinterpret_cast< const Room(*)>(_a[1]))); break;
        case 3: _t->onRoomSelected((*reinterpret_cast< const Room(*)>(_a[1]))); break;
        case 4: _t->onRoomDestroyed((*reinterpret_cast< const Room(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RoomListWidget::*)(const int & , const QString & , const bool & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoomListWidget::cellChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RoomListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RoomListWidget.data,
      qt_meta_data_RoomListWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RoomListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RoomListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RoomListWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RoomListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void RoomListWidget::cellChanged(const int & _t1, const QString & _t2, const bool & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
