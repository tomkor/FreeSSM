/****************************************************************************
** Meta object code from reading C++ file 'CUcontent_DCs_engine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CUcontent_DCs_engine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CUcontent_DCs_engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CUcontent_DCs_engine_t {
    QByteArrayData data[16];
    char stringdata0[326];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUcontent_DCs_engine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUcontent_DCs_engine_t qt_meta_stringdata_CUcontent_DCs_engine = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CUcontent_DCs_engine"
QT_MOC_LITERAL(1, 21, 35), // "updateCurrentOrTemporaryDTCsC..."
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 14), // "currOrTempDTCs"
QT_MOC_LITERAL(4, 73, 25), // "currOrTempDTCdescriptions"
QT_MOC_LITERAL(5, 99, 8), // "testMode"
QT_MOC_LITERAL(6, 108, 12), // "DCheckActive"
QT_MOC_LITERAL(7, 121, 36), // "updateHistoricOrMemorizedDTCs..."
QT_MOC_LITERAL(8, 158, 13), // "histOrMemDTCs"
QT_MOC_LITERAL(9, 172, 24), // "histOrMemDTCdescriptions"
QT_MOC_LITERAL(10, 197, 24), // "updateCClatestCCsContent"
QT_MOC_LITERAL(11, 222, 11), // "latestCCCCs"
QT_MOC_LITERAL(12, 234, 22), // "latestCCCCdescriptions"
QT_MOC_LITERAL(13, 257, 27), // "updateCCmemorizedCCsContent"
QT_MOC_LITERAL(14, 285, 14), // "memorizedCCCCs"
QT_MOC_LITERAL(15, 300, 25) // "memorizedCCCCdescriptions"

    },
    "CUcontent_DCs_engine\0"
    "updateCurrentOrTemporaryDTCsContent\0"
    "\0currOrTempDTCs\0currOrTempDTCdescriptions\0"
    "testMode\0DCheckActive\0"
    "updateHistoricOrMemorizedDTCsContent\0"
    "histOrMemDTCs\0histOrMemDTCdescriptions\0"
    "updateCClatestCCsContent\0latestCCCCs\0"
    "latestCCCCdescriptions\0"
    "updateCCmemorizedCCsContent\0memorizedCCCCs\0"
    "memorizedCCCCdescriptions"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUcontent_DCs_engine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x08 /* Private */,
       7,    2,   43,    2, 0x08 /* Private */,
      10,    2,   48,    2, 0x08 /* Private */,
      13,    2,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList, QMetaType::Bool, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,    8,    9,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,   11,   12,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,   14,   15,

       0        // eod
};

void CUcontent_DCs_engine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUcontent_DCs_engine *_t = static_cast<CUcontent_DCs_engine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateCurrentOrTemporaryDTCsContent((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->updateHistoricOrMemorizedDTCsContent((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 2: _t->updateCClatestCCsContent((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 3: _t->updateCCmemorizedCCsContent((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CUcontent_DCs_engine::staticMetaObject = {
    { &CUcontent_DCs_abstract::staticMetaObject, qt_meta_stringdata_CUcontent_DCs_engine.data,
      qt_meta_data_CUcontent_DCs_engine,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CUcontent_DCs_engine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUcontent_DCs_engine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CUcontent_DCs_engine.stringdata0))
        return static_cast<void*>(this);
    return CUcontent_DCs_abstract::qt_metacast(_clname);
}

int CUcontent_DCs_engine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CUcontent_DCs_abstract::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
