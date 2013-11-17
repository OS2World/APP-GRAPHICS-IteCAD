/****************************************************************************
** Meta object code from reading C++ file 'win_component_prop.h'
**
** Created: Sun Dec 18 11:14:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_component_prop.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_component_prop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_component_prop[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   20,   19,   19, 0x0a,
      51,   19,   19,   19, 0x0a,
      60,   19,   19,   19, 0x0a,
      90,   19,   19,   19, 0x0a,
     120,   19,   19,   19, 0x0a,
     139,   19,   19,   19, 0x0a,
     161,   19,   19,   19, 0x0a,
     181,   19,   19,   19, 0x0a,
     195,   19,   19,   19, 0x0a,
     212,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_component_prop[] = {
    "win_component_prop\0\0index\0"
    "alias_index_changed(int)\0browse()\0"
    "unicode_input_ident_pressed()\0"
    "unicode_input_value_pressed()\0"
    "ungated_pins_add()\0ungated_pins_remove()\0"
    "ungated_pins_edit()\0devices_add()\0"
    "devices_remove()\0devices_edit()\0"
};

const QMetaObject win_component_prop::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_component_prop,
      qt_meta_data_win_component_prop, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_component_prop::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_component_prop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_component_prop::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_component_prop))
        return static_cast<void*>(const_cast< win_component_prop*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_component_prop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: alias_index_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: browse(); break;
        case 2: unicode_input_ident_pressed(); break;
        case 3: unicode_input_value_pressed(); break;
        case 4: ungated_pins_add(); break;
        case 5: ungated_pins_remove(); break;
        case 6: ungated_pins_edit(); break;
        case 7: devices_add(); break;
        case 8: devices_remove(); break;
        case 9: devices_edit(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
