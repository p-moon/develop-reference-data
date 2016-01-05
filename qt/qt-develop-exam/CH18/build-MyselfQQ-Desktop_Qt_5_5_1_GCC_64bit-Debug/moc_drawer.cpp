/****************************************************************************
** Meta object code from reading C++ file 'drawer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MyselfQQ/drawer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'drawer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Drawer_t {
    QByteArrayData data[11];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Drawer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Drawer_t qt_meta_stringdata_Drawer = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Drawer"
QT_MOC_LITERAL(1, 7, 15), // "showChatWidget1"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 15), // "showChatWidget2"
QT_MOC_LITERAL(4, 40, 15), // "showChatWidget3"
QT_MOC_LITERAL(5, 56, 15), // "showChatWidget4"
QT_MOC_LITERAL(6, 72, 15), // "showChatWidget5"
QT_MOC_LITERAL(7, 88, 15), // "showChatWidget6"
QT_MOC_LITERAL(8, 104, 15), // "showChatWidget7"
QT_MOC_LITERAL(9, 120, 15), // "showChatWidget8"
QT_MOC_LITERAL(10, 136, 15) // "showChatWidget9"

    },
    "Drawer\0showChatWidget1\0\0showChatWidget2\0"
    "showChatWidget3\0showChatWidget4\0"
    "showChatWidget5\0showChatWidget6\0"
    "showChatWidget7\0showChatWidget8\0"
    "showChatWidget9"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Drawer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Drawer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Drawer *_t = static_cast<Drawer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showChatWidget1(); break;
        case 1: _t->showChatWidget2(); break;
        case 2: _t->showChatWidget3(); break;
        case 3: _t->showChatWidget4(); break;
        case 4: _t->showChatWidget5(); break;
        case 5: _t->showChatWidget6(); break;
        case 6: _t->showChatWidget7(); break;
        case 7: _t->showChatWidget8(); break;
        case 8: _t->showChatWidget9(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Drawer::staticMetaObject = {
    { &QToolBox::staticMetaObject, qt_meta_stringdata_Drawer.data,
      qt_meta_data_Drawer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Drawer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Drawer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Drawer.stringdata0))
        return static_cast<void*>(const_cast< Drawer*>(this));
    return QToolBox::qt_metacast(_clname);
}

int Drawer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
