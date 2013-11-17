/****************************************************************************
** Meta object code from reading C++ file 'win_print.h'
**
** Created: Sun Dec 18 11:14:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_print.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_print.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_print[] = {

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
      11,   10,   10,   10, 0x0a,
      36,   28,   10,   10, 0x0a,
      59,   28,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_print[] = {
    "win_print\0\0printer_dialog()\0inutile\0"
    "change_pcb_scale(bool)\0change_colormode(bool)\0"
};

const QMetaObject win_print::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_print,
      qt_meta_data_win_print, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_print::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_print::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_print::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_print))
        return static_cast<void*>(const_cast< win_print*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_print::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: printer_dialog(); break;
        case 1: change_pcb_scale((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: change_colormode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
