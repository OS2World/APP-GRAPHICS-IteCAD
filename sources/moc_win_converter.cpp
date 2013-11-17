/****************************************************************************
** Meta object code from reading C++ file 'win_converter.h'
**
** Created: Sun Dec 18 11:14:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_converter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_converter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_converter[] = {

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
      23,   15,   14,   14, 0x0a,
      49,   41,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_converter[] = {
    "win_converter\0\0stringa\0T_edited(QString)\0"
    "inutile\0U_change(int)\0"
};

const QMetaObject win_converter::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_converter,
      qt_meta_data_win_converter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_converter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_converter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_converter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_converter))
        return static_cast<void*>(const_cast< win_converter*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_converter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: T_edited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: U_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
