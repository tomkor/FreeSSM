/****************************************************************************
** Meta object code from reading C++ file 'CUcontent_DCs_stopCodes.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CUcontent_DCs_stopCodes.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CUcontent_DCs_stopCodes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CUcontent_DCs_stopCodes_t {
    QByteArrayData data[5];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUcontent_DCs_stopCodes_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUcontent_DCs_stopCodes_t qt_meta_stringdata_CUcontent_DCs_stopCodes = {
    {
QT_MOC_LITERAL(0, 0, 23), // "CUcontent_DCs_stopCodes"
QT_MOC_LITERAL(1, 24, 35), // "updateCurrentOrTemporaryDTCsC..."
QT_MOC_LITERAL(2, 60, 0), // ""
QT_MOC_LITERAL(3, 61, 14), // "currOrTempDTCs"
QT_MOC_LITERAL(4, 76, 25) // "currOrTempDTCdescriptions"

    },
    "CUcontent_DCs_stopCodes\0"
    "updateCurrentOrTemporaryDTCsContent\0"
    "\0currOrTempDTCs\0currOrTempDTCdescriptions"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUcontent_DCs_stopCodes[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,    3,    4,

       0        // eod
};

void CUcontent_DCs_stopCodes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUcontent_DCs_stopCodes *_t = static_cast<CUcontent_DCs_stopCodes *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateCurrentOrTemporaryDTCsContent((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CUcontent_DCs_stopCodes::staticMetaObject = {
    { &CUcontent_DCs_abstract::staticMetaObject, qt_meta_stringdata_CUcontent_DCs_stopCodes.data,
      qt_meta_data_CUcontent_DCs_stopCodes,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CUcontent_DCs_stopCodes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUcontent_DCs_stopCodes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CUcontent_DCs_stopCodes.stringdata0))
        return static_cast<void*>(this);
    return CUcontent_DCs_abstract::qt_metacast(_clname);
}

int CUcontent_DCs_stopCodes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CUcontent_DCs_abstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
