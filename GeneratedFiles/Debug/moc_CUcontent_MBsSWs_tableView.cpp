/****************************************************************************
** Meta object code from reading C++ file 'CUcontent_MBsSWs_tableView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CUcontent_MBsSWs_tableView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CUcontent_MBsSWs_tableView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CUcontent_MBsSWs_tableView_t {
    QByteArrayData data[10];
    char stringdata0[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUcontent_MBsSWs_tableView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUcontent_MBsSWs_tableView_t qt_meta_stringdata_CUcontent_MBsSWs_tableView = {
    {
QT_MOC_LITERAL(0, 0, 26), // "CUcontent_MBsSWs_tableView"
QT_MOC_LITERAL(1, 27, 20), // "moveUpButton_pressed"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 22), // "moveDownButton_pressed"
QT_MOC_LITERAL(4, 72, 25), // "resetMinMaxButton_pressed"
QT_MOC_LITERAL(5, 98, 20), // "itemSelectionChanged"
QT_MOC_LITERAL(6, 119, 26), // "setMoveButtonsEnabledState"
QT_MOC_LITERAL(7, 146, 22), // "toggleMinColumnVisible"
QT_MOC_LITERAL(8, 169, 4), // "show"
QT_MOC_LITERAL(9, 174, 22) // "toggleMaxColumnVisible"

    },
    "CUcontent_MBsSWs_tableView\0"
    "moveUpButton_pressed\0\0moveDownButton_pressed\0"
    "resetMinMaxButton_pressed\0"
    "itemSelectionChanged\0setMoveButtonsEnabledState\0"
    "toggleMinColumnVisible\0show\0"
    "toggleMaxColumnVisible"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUcontent_MBsSWs_tableView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    0,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   53,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       9,    1,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void CUcontent_MBsSWs_tableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUcontent_MBsSWs_tableView *_t = static_cast<CUcontent_MBsSWs_tableView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->moveUpButton_pressed(); break;
        case 1: _t->moveDownButton_pressed(); break;
        case 2: _t->resetMinMaxButton_pressed(); break;
        case 3: _t->itemSelectionChanged(); break;
        case 4: _t->setMoveButtonsEnabledState(); break;
        case 5: _t->toggleMinColumnVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->toggleMaxColumnVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CUcontent_MBsSWs_tableView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CUcontent_MBsSWs_tableView::moveUpButton_pressed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CUcontent_MBsSWs_tableView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CUcontent_MBsSWs_tableView::moveDownButton_pressed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CUcontent_MBsSWs_tableView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CUcontent_MBsSWs_tableView::resetMinMaxButton_pressed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CUcontent_MBsSWs_tableView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CUcontent_MBsSWs_tableView::itemSelectionChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CUcontent_MBsSWs_tableView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CUcontent_MBsSWs_tableView.data,
      qt_meta_data_CUcontent_MBsSWs_tableView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CUcontent_MBsSWs_tableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUcontent_MBsSWs_tableView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CUcontent_MBsSWs_tableView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CUcontent_MBsSWs_tableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CUcontent_MBsSWs_tableView::moveUpButton_pressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CUcontent_MBsSWs_tableView::moveDownButton_pressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CUcontent_MBsSWs_tableView::resetMinMaxButton_pressed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CUcontent_MBsSWs_tableView::itemSelectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
