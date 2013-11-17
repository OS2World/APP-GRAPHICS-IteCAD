/****************************************************************************
** Meta object code from reading C++ file 'win_style.h'
**
** Created: Sun Dec 18 11:14:19 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_style.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_style.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_style[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   11,   10,   10, 0x0a,
      38,   36,   10,   10, 0x0a,
      59,   10,   10,   10, 0x0a,
      73,   10,   10,   10, 0x0a,
      89,   10,   10,   10, 0x0a,
     115,  108,   10,   10, 0x0a,
     157,   10,   10,   10, 0x0a,
     171,   10,   10,   10, 0x0a,
     187,   10,   10,   10, 0x0a,
     202,   10,   10,   10, 0x0a,
     219,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_style[] = {
    "win_style\0\0f\0change_family(QString)\0"
    "p\0change_size(QString)\0change_bold()\0"
    "change_italic()\0change_underline()\0"
    "format\0currentCharFormatChanged(QTextCharFormat)\0"
    "change_left()\0change_center()\0"
    "change_right()\0change_justify()\0"
    "cursorPositionChanged()\0"
};

const QMetaObject win_style::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_style,
      qt_meta_data_win_style, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_style::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_style::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_style::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_style))
        return static_cast<void*>(const_cast< win_style*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_style::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: change_family((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: change_size((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: change_bold(); break;
        case 3: change_italic(); break;
        case 4: change_underline(); break;
        case 5: currentCharFormatChanged((*reinterpret_cast< const QTextCharFormat(*)>(_a[1]))); break;
        case 6: change_left(); break;
        case 7: change_center(); break;
        case 8: change_right(); break;
        case 9: change_justify(); break;
        case 10: cursorPositionChanged(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
