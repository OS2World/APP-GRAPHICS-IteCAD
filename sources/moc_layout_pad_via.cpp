/****************************************************************************
** Meta object code from reading C++ file 'layout_pad_via.h'
**
** Created: Sun Dec 18 11:15:28 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "layout_pad_via.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'layout_pad_via.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_layout_pad_via[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   16,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_layout_pad_via[] = {
    "layout_pad_via\0\0new_index\0change_type(int)\0"
};

const QMetaObject layout_pad_via::staticMetaObject = {
    { &QGridLayout::staticMetaObject, qt_meta_stringdata_layout_pad_via,
      qt_meta_data_layout_pad_via, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &layout_pad_via::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *layout_pad_via::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *layout_pad_via::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_layout_pad_via))
        return static_cast<void*>(const_cast< layout_pad_via*>(this));
    return QGridLayout::qt_metacast(_clname);
}

int layout_pad_via::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGridLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: change_type((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
