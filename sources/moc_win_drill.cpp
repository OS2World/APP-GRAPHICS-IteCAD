/****************************************************************************
** Meta object code from reading C++ file 'win_drill.h'
**
** Created: Sun Dec 18 11:15:44 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_drill.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_drill.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_drill[] = {

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
      11,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_drill[] = {
    "win_drill\0\0change_directory()\0"
};

const QMetaObject win_drill::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_drill,
      qt_meta_data_win_drill, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_drill::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_drill::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_drill::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_drill))
        return static_cast<void*>(const_cast< win_drill*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_drill::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: change_directory(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
