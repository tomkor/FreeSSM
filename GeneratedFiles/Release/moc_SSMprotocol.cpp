/****************************************************************************
** Meta object code from reading C++ file 'SSMprotocol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/SSMprotocol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SSMprotocol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SSMprotocol_t {
    QByteArrayData data[33];
    char stringdata0[530];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SSMprotocol_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SSMprotocol_t qt_meta_stringdata_SSMprotocol = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SSMprotocol"
QT_MOC_LITERAL(1, 12, 22), // "currentOrTemporaryDTCs"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 11), // "currentDTCs"
QT_MOC_LITERAL(4, 48, 23), // "currentDTCsDescriptions"
QT_MOC_LITERAL(5, 72, 8), // "testMode"
QT_MOC_LITERAL(6, 81, 12), // "DCheckActive"
QT_MOC_LITERAL(7, 94, 23), // "historicOrMemorizedDTCs"
QT_MOC_LITERAL(8, 118, 12), // "historicDTCs"
QT_MOC_LITERAL(9, 131, 24), // "historicDTCsDescriptions"
QT_MOC_LITERAL(10, 156, 11), // "latestCCCCs"
QT_MOC_LITERAL(11, 168, 12), // "currentCCCCs"
QT_MOC_LITERAL(12, 181, 24), // "currentCCCCsDescriptions"
QT_MOC_LITERAL(13, 206, 14), // "memorizedCCCCs"
QT_MOC_LITERAL(14, 221, 13), // "historicCCCCs"
QT_MOC_LITERAL(15, 235, 25), // "historicCCCCsDescriptions"
QT_MOC_LITERAL(16, 261, 16), // "newMBSWrawValues"
QT_MOC_LITERAL(17, 278, 17), // "std::vector<uint>"
QT_MOC_LITERAL(18, 296, 9), // "rawValues"
QT_MOC_LITERAL(19, 306, 11), // "duration_ms"
QT_MOC_LITERAL(20, 318, 16), // "startedDCreading"
QT_MOC_LITERAL(21, 335, 18), // "startedMBSWreading"
QT_MOC_LITERAL(22, 354, 19), // "startedActuatorTest"
QT_MOC_LITERAL(23, 374, 16), // "stoppedDCreading"
QT_MOC_LITERAL(24, 391, 18), // "stoppedMBSWreading"
QT_MOC_LITERAL(25, 410, 19), // "stoppedActuatorTest"
QT_MOC_LITERAL(26, 430, 9), // "commError"
QT_MOC_LITERAL(27, 440, 18), // "processMBSWrawData"
QT_MOC_LITERAL(28, 459, 17), // "std::vector<char>"
QT_MOC_LITERAL(29, 477, 11), // "MBSWrawdata"
QT_MOC_LITERAL(30, 489, 18), // "processDTCsRawdata"
QT_MOC_LITERAL(31, 508, 9), // "dcrawdata"
QT_MOC_LITERAL(32, 518, 11) // "resetCUdata"

    },
    "SSMprotocol\0currentOrTemporaryDTCs\0\0"
    "currentDTCs\0currentDTCsDescriptions\0"
    "testMode\0DCheckActive\0historicOrMemorizedDTCs\0"
    "historicDTCs\0historicDTCsDescriptions\0"
    "latestCCCCs\0currentCCCCs\0"
    "currentCCCCsDescriptions\0memorizedCCCCs\0"
    "historicCCCCs\0historicCCCCsDescriptions\0"
    "newMBSWrawValues\0std::vector<uint>\0"
    "rawValues\0duration_ms\0startedDCreading\0"
    "startedMBSWreading\0startedActuatorTest\0"
    "stoppedDCreading\0stoppedMBSWreading\0"
    "stoppedActuatorTest\0commError\0"
    "processMBSWrawData\0std::vector<char>\0"
    "MBSWrawdata\0processDTCsRawdata\0dcrawdata\0"
    "resetCUdata"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SSMprotocol[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   89,    2, 0x06 /* Public */,
       7,    2,   98,    2, 0x06 /* Public */,
      10,    2,  103,    2, 0x06 /* Public */,
      13,    2,  108,    2, 0x06 /* Public */,
      16,    2,  113,    2, 0x06 /* Public */,
      20,    0,  118,    2, 0x06 /* Public */,
      21,    0,  119,    2, 0x06 /* Public */,
      22,    0,  120,    2, 0x06 /* Public */,
      23,    0,  121,    2, 0x06 /* Public */,
      24,    0,  122,    2, 0x06 /* Public */,
      25,    0,  123,    2, 0x06 /* Public */,
      26,    0,  124,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      27,    2,  125,    2, 0x09 /* Protected */,
      30,    2,  130,    2, 0x09 /* Protected */,
      32,    0,  135,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList, QMetaType::Bool, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,    8,    9,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,   11,   12,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,   14,   15,
    QMetaType::Void, 0x80000000 | 17, QMetaType::Int,   18,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 28, QMetaType::Int,   29,   19,
    QMetaType::UInt, 0x80000000 | 28, QMetaType::Int,   31,   19,
    QMetaType::Void,

       0        // eod
};

void SSMprotocol::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SSMprotocol *_t = static_cast<SSMprotocol *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->currentOrTemporaryDTCs((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->historicOrMemorizedDTCs((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 2: _t->latestCCCCs((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 3: _t->memorizedCCCCs((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 4: _t->newMBSWrawValues((*reinterpret_cast< const std::vector<uint>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->startedDCreading(); break;
        case 6: _t->startedMBSWreading(); break;
        case 7: _t->startedActuatorTest(); break;
        case 8: _t->stoppedDCreading(); break;
        case 9: _t->stoppedMBSWreading(); break;
        case 10: _t->stoppedActuatorTest(); break;
        case 11: _t->commError(); break;
        case 12: _t->processMBSWrawData((*reinterpret_cast< const std::vector<char>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: { uint _r = _t->processDTCsRawdata((*reinterpret_cast< std::vector<char>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->resetCUdata(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SSMprotocol::*)(QStringList , QStringList , bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::currentOrTemporaryDTCs)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)(QStringList , QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::historicOrMemorizedDTCs)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)(QStringList , QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::latestCCCCs)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)(QStringList , QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::memorizedCCCCs)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)(const std::vector<unsigned int> & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::newMBSWrawValues)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::startedDCreading)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::startedMBSWreading)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::startedActuatorTest)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::stoppedDCreading)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::stoppedMBSWreading)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::stoppedActuatorTest)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (SSMprotocol::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SSMprotocol::commError)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SSMprotocol::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SSMprotocol.data,
      qt_meta_data_SSMprotocol,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SSMprotocol::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SSMprotocol::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SSMprotocol.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SSMprotocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void SSMprotocol::currentOrTemporaryDTCs(QStringList _t1, QStringList _t2, bool _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SSMprotocol::historicOrMemorizedDTCs(QStringList _t1, QStringList _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SSMprotocol::latestCCCCs(QStringList _t1, QStringList _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SSMprotocol::memorizedCCCCs(QStringList _t1, QStringList _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SSMprotocol::newMBSWrawValues(const std::vector<unsigned int> & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SSMprotocol::startedDCreading()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SSMprotocol::startedMBSWreading()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SSMprotocol::startedActuatorTest()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void SSMprotocol::stoppedDCreading()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void SSMprotocol::stoppedMBSWreading()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void SSMprotocol::stoppedActuatorTest()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void SSMprotocol::commError()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
