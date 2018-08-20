/****************************************************************************
** Meta object code from reading C++ file 'Preferences.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/Preferences.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Preferences.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Preferences_t {
    QByteArrayData data[15];
    char stringdata0[174];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Preferences_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Preferences_t qt_meta_stringdata_Preferences = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Preferences"
QT_MOC_LITERAL(1, 12, 18), // "languageSelChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 8), // "language"
QT_MOC_LITERAL(4, 41, 12), // "QTranslator*"
QT_MOC_LITERAL(5, 54, 10), // "translator"
QT_MOC_LITERAL(6, 65, 14), // "switchLanguage"
QT_MOC_LITERAL(7, 80, 9), // "langindex"
QT_MOC_LITERAL(8, 90, 14), // "switchGUIstyle"
QT_MOC_LITERAL(9, 105, 5), // "style"
QT_MOC_LITERAL(10, 111, 19), // "selectInterfaceType"
QT_MOC_LITERAL(11, 131, 5), // "index"
QT_MOC_LITERAL(12, 137, 19), // "selectInterfaceName"
QT_MOC_LITERAL(13, 157, 13), // "interfacetest"
QT_MOC_LITERAL(14, 171, 2) // "ok"

    },
    "Preferences\0languageSelChanged\0\0"
    "language\0QTranslator*\0translator\0"
    "switchLanguage\0langindex\0switchGUIstyle\0"
    "style\0selectInterfaceType\0index\0"
    "selectInterfaceName\0interfacetest\0ok"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Preferences[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   54,    2, 0x0a /* Public */,
       8,    1,   57,    2, 0x0a /* Public */,
      10,    1,   60,    2, 0x0a /* Public */,
      12,    1,   63,    2, 0x0a /* Public */,
      13,    0,   66,    2, 0x0a /* Public */,
      14,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Preferences::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Preferences *_t = static_cast<Preferences *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->languageSelChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QTranslator*(*)>(_a[2]))); break;
        case 1: _t->switchLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->switchGUIstyle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->selectInterfaceType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->selectInterfaceName((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->interfacetest(); break;
        case 6: _t->ok(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTranslator* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Preferences::*)(QString , QTranslator * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Preferences::languageSelChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Preferences::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Preferences.data,
      qt_meta_data_Preferences,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Preferences::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Preferences::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Preferences.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Preferences::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Preferences::languageSelChanged(QString _t1, QTranslator * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
