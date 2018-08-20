/****************************************************************************
** Meta object code from reading C++ file 'CUcontent_DCs_abstract.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CUcontent_DCs_abstract.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CUcontent_DCs_abstract.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CUcontent_DCs_abstract_t {
    QByteArrayData data[6];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUcontent_DCs_abstract_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUcontent_DCs_abstract_t qt_meta_stringdata_CUcontent_DCs_abstract = {
    {
QT_MOC_LITERAL(0, 0, 22), // "CUcontent_DCs_abstract"
QT_MOC_LITERAL(1, 23, 5), // "error"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 9), // "callStart"
QT_MOC_LITERAL(4, 40, 8), // "callStop"
QT_MOC_LITERAL(5, 49, 15) // "printDCprotocol"

    },
    "CUcontent_DCs_abstract\0error\0\0callStart\0"
    "callStop\0printDCprotocol"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUcontent_DCs_abstract[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x09 /* Protected */,
       4,    0,   36,    2, 0x09 /* Protected */,
       5,    0,   37,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CUcontent_DCs_abstract::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUcontent_DCs_abstract *_t = static_cast<CUcontent_DCs_abstract *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->error(); break;
        case 1: _t->callStart(); break;
        case 2: _t->callStop(); break;
        case 3: _t->printDCprotocol(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CUcontent_DCs_abstract::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CUcontent_DCs_abstract::error)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CUcontent_DCs_abstract::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CUcontent_DCs_abstract.data,
      qt_meta_data_CUcontent_DCs_abstract,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CUcontent_DCs_abstract::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUcontent_DCs_abstract::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CUcontent_DCs_abstract.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CUcontent_DCs_abstract::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CUcontent_DCs_abstract::error()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
