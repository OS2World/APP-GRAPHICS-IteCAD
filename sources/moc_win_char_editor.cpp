/****************************************************************************
** Meta object code from reading C++ file 'win_char_editor.h'
**
** Created: Sun Dec 18 11:15:06 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_char_editor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_char_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_win_char_editor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x0a,
      43,   17,   16,   16, 0x0a,
      63,   16,   16,   16, 0x0a,
      74,   16,   16,   16, 0x0a,
      86,   16,   16,   16, 0x0a,
      98,   16,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,
     125,   16,   16,   16, 0x0a,
     137,   16,   16,   16, 0x0a,
     164,  154,   16,   16, 0x0a,
     192,  154,   16,   16, 0x0a,
     226,  220,   16,   16, 0x0a,
     255,  249,   16,   16, 0x0a,
     279,   16,   16,   16, 0x0a,
     296,  249,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_char_editor[] = {
    "win_char_editor\0\0flag\0set_tool_erase(bool)\0"
    "set_tool_draw(bool)\0file_new()\0"
    "file_open()\0file_save()\0file_save_as()\0"
    "edit_undo()\0edit_redo()\0menu_file_open()\0"
    "new_value\0edit_can_undo_changed(bool)\0"
    "edit_can_redo_changed(bool)\0state\0"
    "background_on_off(int)\0index\0"
    "background_changed(int)\0actions_escape()\0"
    "char_width_changed(int)\0"
};

const QMetaObject win_char_editor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_char_editor,
      qt_meta_data_win_char_editor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_char_editor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_char_editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_char_editor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_char_editor))
        return static_cast<void*>(const_cast< win_char_editor*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_char_editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: set_tool_erase((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: set_tool_draw((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: file_new(); break;
        case 3: file_open(); break;
        case 4: file_save(); break;
        case 5: file_save_as(); break;
        case 6: edit_undo(); break;
        case 7: edit_redo(); break;
        case 8: menu_file_open(); break;
        case 9: edit_can_undo_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: edit_can_redo_changed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: background_on_off((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: background_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: actions_escape(); break;
        case 14: char_width_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
