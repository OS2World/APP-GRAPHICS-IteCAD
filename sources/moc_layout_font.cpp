/****************************************************************************
** Meta object code from reading C++ file 'layout_font.h'
**
** Created: Sun Dec 18 11:14:15 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "layout_font.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'layout_font.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_layout_font[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x0a,
      47,   39,   12,   12, 0x0a,
      66,   39,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_layout_font[] = {
    "layout_font\0\0index\0height_changed(int)\0"
    "checked\0bold_changed(bool)\0"
    "italic_changed(bool)\0"
};

const QMetaObject layout_font::staticMetaObject = {
    { &QHBoxLayout::staticMetaObject, qt_meta_stringdata_layout_font,
      qt_meta_data_layout_font, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &layout_font::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *layout_font::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *layout_font::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_layout_font))
        return static_cast<void*>(const_cast< layout_font*>(this));
    return QHBoxLayout::qt_metacast(_clname);
}

int layout_font::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QHBoxLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: height_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: bold_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: italic_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
