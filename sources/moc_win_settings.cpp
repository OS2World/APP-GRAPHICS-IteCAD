/****************************************************************************
** Meta object code from reading C++ file 'win_settings.h'
**
** Created: Sun Dec 18 11:15:22 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_settings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_settings[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      38,   13,   13,   13, 0x0a,
      75,   70,   13,   13, 0x0a,
     107,   13,   13,   13, 0x0a,
     125,  117,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_settings[] = {
    "win_settings\0\0load_factory_settings()\0"
    "load_general_default_settings()\0item\0"
    "change_color(QTableWidgetItem*)\0"
    "timeout()\0inutile\0grid_UM_changed(int)\0"
};

const QMetaObject win_settings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_settings,
      qt_meta_data_win_settings, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_settings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_settings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_settings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_settings))
        return static_cast<void*>(const_cast< win_settings*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_settings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: load_factory_settings(); break;
        case 1: load_general_default_settings(); break;
        case 2: change_color((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: timeout(); break;
        case 4: grid_UM_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
