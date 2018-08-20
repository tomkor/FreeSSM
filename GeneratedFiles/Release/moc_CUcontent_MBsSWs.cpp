/****************************************************************************
** Meta object code from reading C++ file 'CUcontent_MBsSWs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CUcontent_MBsSWs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CUcontent_MBsSWs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CUcontent_MBsSWs_t {
    QByteArrayData data[19];
    char stringdata0[299];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUcontent_MBsSWs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUcontent_MBsSWs_t qt_meta_stringdata_CUcontent_MBsSWs = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CUcontent_MBsSWs"
QT_MOC_LITERAL(1, 17, 5), // "error"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 28), // "startstopMBsSWsButtonPressed"
QT_MOC_LITERAL(4, 53, 9), // "callStart"
QT_MOC_LITERAL(5, 63, 8), // "callStop"
QT_MOC_LITERAL(6, 72, 20), // "processMBSWRawValues"
QT_MOC_LITERAL(7, 93, 17), // "std::vector<uint>"
QT_MOC_LITERAL(8, 111, 9), // "rawValues"
QT_MOC_LITERAL(9, 121, 18), // "refreshduration_ms"
QT_MOC_LITERAL(10, 140, 9), // "addMBsSWs"
QT_MOC_LITERAL(11, 150, 12), // "deleteMBsSWs"
QT_MOC_LITERAL(12, 163, 10), // "saveMBsSWs"
QT_MOC_LITERAL(13, 174, 10), // "loadMBsSWs"
QT_MOC_LITERAL(14, 185, 22), // "moveUpMBsSWsOnTheTable"
QT_MOC_LITERAL(15, 208, 24), // "moveDownMBsSWsOnTheTable"
QT_MOC_LITERAL(16, 233, 22), // "resetMinMaxTableValues"
QT_MOC_LITERAL(17, 256, 27), // "setDeleteButtonEnabledState"
QT_MOC_LITERAL(18, 284, 14) // "switchTimeMode"

    },
    "CUcontent_MBsSWs\0error\0\0"
    "startstopMBsSWsButtonPressed\0callStart\0"
    "callStop\0processMBSWRawValues\0"
    "std::vector<uint>\0rawValues\0"
    "refreshduration_ms\0addMBsSWs\0deleteMBsSWs\0"
    "saveMBsSWs\0loadMBsSWs\0moveUpMBsSWsOnTheTable\0"
    "moveDownMBsSWsOnTheTable\0"
    "resetMinMaxTableValues\0"
    "setDeleteButtonEnabledState\0switchTimeMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUcontent_MBsSWs[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    2,   88,    2, 0x08 /* Private */,
      10,    0,   93,    2, 0x08 /* Private */,
      11,    0,   94,    2, 0x08 /* Private */,
      12,    0,   95,    2, 0x08 /* Private */,
      13,    0,   96,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    0,   98,    2, 0x08 /* Private */,
      16,    0,   99,    2, 0x08 /* Private */,
      17,    0,  100,    2, 0x08 /* Private */,
      18,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
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

void CUcontent_MBsSWs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUcontent_MBsSWs *_t = static_cast<CUcontent_MBsSWs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->error(); break;
        case 1: _t->startstopMBsSWsButtonPressed(); break;
        case 2: _t->callStart(); break;
        case 3: _t->callStop(); break;
        case 4: _t->processMBSWRawValues((*reinterpret_cast< const std::vector<uint>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->addMBsSWs(); break;
        case 6: _t->deleteMBsSWs(); break;
        case 7: _t->saveMBsSWs(); break;
        case 8: _t->loadMBsSWs(); break;
        case 9: _t->moveUpMBsSWsOnTheTable(); break;
        case 10: _t->moveDownMBsSWsOnTheTable(); break;
        case 11: _t->resetMinMaxTableValues(); break;
        case 12: _t->setDeleteButtonEnabledState(); break;
        case 13: _t->switchTimeMode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CUcontent_MBsSWs::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CUcontent_MBsSWs::error)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CUcontent_MBsSWs::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CUcontent_MBsSWs.data,
      qt_meta_data_CUcontent_MBsSWs,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CUcontent_MBsSWs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUcontent_MBsSWs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CUcontent_MBsSWs.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CUcontent_MBsSWs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CUcontent_MBsSWs::error()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
