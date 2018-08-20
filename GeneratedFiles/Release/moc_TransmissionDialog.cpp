/****************************************************************************
** Meta object code from reading C++ file 'TransmissionDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/TransmissionDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TransmissionDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TransmissionDialog_t {
    QByteArrayData data[7];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TransmissionDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TransmissionDialog_t qt_meta_stringdata_TransmissionDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TransmissionDialog"
QT_MOC_LITERAL(1, 19, 4), // "DTCs"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 15), // "measuringblocks"
QT_MOC_LITERAL(4, 41, 11), // "adjustments"
QT_MOC_LITERAL(5, 53, 11), // "clearMemory"
QT_MOC_LITERAL(6, 65, 12) // "clearMemory2"

    },
    "TransmissionDialog\0DTCs\0\0measuringblocks\0"
    "adjustments\0clearMemory\0clearMemory2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TransmissionDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TransmissionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TransmissionDialog *_t = static_cast<TransmissionDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DTCs(); break;
        case 1: _t->measuringblocks(); break;
        case 2: _t->adjustments(); break;
        case 3: _t->clearMemory(); break;
        case 4: _t->clearMemory2(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TransmissionDialog::staticMetaObject = {
    { &ControlUnitDialog::staticMetaObject, qt_meta_stringdata_TransmissionDialog.data,
      qt_meta_data_TransmissionDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TransmissionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransmissionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TransmissionDialog.stringdata0))
        return static_cast<void*>(this);
    return ControlUnitDialog::qt_metacast(_clname);
}

int TransmissionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ControlUnitDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
