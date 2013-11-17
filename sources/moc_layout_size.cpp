/****************************************************************************
** Meta object code from reading C++ file 'layout_size.h'
**
** Created: Sun Dec 18 11:14:16 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "layout_size.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'layout_size.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_layout_size[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   13,   12,   12, 0x0a,
      57,   51,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_layout_size[] = {
    "layout_size\0\0checked\0change_page_orientation(bool)\0"
    "index\0change_page_size(int)\0"
};

const QMetaObject layout_size::staticMetaObject = {
    { &QGridLayout::staticMetaObject, qt_meta_stringdata_layout_size,
      qt_meta_data_layout_size, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &layout_size::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *layout_size::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *layout_size::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_layout_size))
        return static_cast<void*>(const_cast< layout_size*>(this));
    return QGridLayout::qt_metacast(_clname);
}

int layout_size::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGridLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: change_page_orientation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: change_page_size((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
