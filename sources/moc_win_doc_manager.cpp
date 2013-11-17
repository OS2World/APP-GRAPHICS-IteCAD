/****************************************************************************
** Meta object code from reading C++ file 'win_doc_manager.h'
**
** Created: Sun Dec 18 11:14:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_doc_manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_doc_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_doc_manager[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      30,   16,   16,   16, 0x0a,
      43,   16,   16,   16, 0x0a,
      49,   16,   16,   16, 0x0a,
      58,   16,   16,   16, 0x0a,
      73,   69,   16,   16, 0x0a,
      96,   90,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_doc_manager[] = {
    "win_doc_manager\0\0remove_doc()\0"
    "rename_doc()\0add()\0up_doc()\0down_doc()\0"
    "row\0changed_row(int)\0punto\0"
    "popup_menu(QPoint)\0"
};

const QMetaObject win_doc_manager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_doc_manager,
      qt_meta_data_win_doc_manager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_doc_manager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_doc_manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_doc_manager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_doc_manager))
        return static_cast<void*>(const_cast< win_doc_manager*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_doc_manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: remove_doc(); break;
        case 1: rename_doc(); break;
        case 2: add(); break;
        case 3: up_doc(); break;
        case 4: down_doc(); break;
        case 5: changed_row((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: popup_menu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
