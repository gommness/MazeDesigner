/****************************************************************************
** Meta object code from reading C++ file 'instancecanvas.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MazeDesignerGUI/instancecanvas.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'instancecanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InstanceCanvas_t {
    QByteArrayData data[14];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InstanceCanvas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InstanceCanvas_t qt_meta_stringdata_InstanceCanvas = {
    {
QT_MOC_LITERAL(0, 0, 14), // "InstanceCanvas"
QT_MOC_LITERAL(1, 15, 6), // "select"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 18), // "SelectableInstance"
QT_MOC_LITERAL(4, 42, 4), // "inst"
QT_MOC_LITERAL(5, 47, 10), // "selectDoor"
QT_MOC_LITERAL(6, 58, 13), // "DoorInstance&"
QT_MOC_LITERAL(7, 72, 4), // "door"
QT_MOC_LITERAL(8, 77, 14), // "clearSelection"
QT_MOC_LITERAL(9, 92, 17), // "onKeyModelDeleted"
QT_MOC_LITERAL(10, 110, 3), // "Key"
QT_MOC_LITERAL(11, 114, 5), // "model"
QT_MOC_LITERAL(12, 120, 16), // "onSpaceDestroyed"
QT_MOC_LITERAL(13, 137, 4) // "poly"

    },
    "InstanceCanvas\0select\0\0SelectableInstance\0"
    "inst\0selectDoor\0DoorInstance&\0door\0"
    "clearSelection\0onKeyModelDeleted\0Key\0"
    "model\0onSpaceDestroyed\0poly"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InstanceCanvas[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       8,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   46,    2, 0x09 /* Protected */,
      12,    1,   49,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QPolygonF,   13,

       0        // eod
};

void InstanceCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InstanceCanvas *_t = static_cast<InstanceCanvas *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->select((*reinterpret_cast< const SelectableInstance(*)>(_a[1]))); break;
        case 1: _t->selectDoor((*reinterpret_cast< DoorInstance(*)>(_a[1]))); break;
        case 2: _t->clearSelection(); break;
        case 3: _t->onKeyModelDeleted((*reinterpret_cast< const Key(*)>(_a[1]))); break;
        case 4: _t->onSpaceDestroyed((*reinterpret_cast< const QPolygonF(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InstanceCanvas::*)(const SelectableInstance & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InstanceCanvas::select)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (InstanceCanvas::*)(DoorInstance & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InstanceCanvas::selectDoor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (InstanceCanvas::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InstanceCanvas::clearSelection)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InstanceCanvas::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_InstanceCanvas.data,
      qt_meta_data_InstanceCanvas,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *InstanceCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InstanceCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InstanceCanvas.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int InstanceCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void InstanceCanvas::select(const SelectableInstance & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InstanceCanvas::selectDoor(DoorInstance & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void InstanceCanvas::clearSelection()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
