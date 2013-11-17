/****************************************************************************
** Meta object code from reading C++ file 'win_xml_editor.h'
**
** Created: Sun Dec 18 11:15:09 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_xml_editor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_xml_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Highlighter[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Highlighter[] = {
    "Highlighter\0"
};

const QMetaObject Highlighter::staticMetaObject = {
    { &QSyntaxHighlighter::staticMetaObject, qt_meta_stringdata_Highlighter,
      qt_meta_data_Highlighter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Highlighter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Highlighter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Highlighter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Highlighter))
        return static_cast<void*>(const_cast< Highlighter*>(this));
    return QSyntaxHighlighter::qt_metacast(_clname);
}

int Highlighter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSyntaxHighlighter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_win_xml_editor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      32,   15,   15,   15, 0x0a,
      48,   15,   15,   15, 0x0a,
      64,   15,   15,   15, 0x0a,
      76,   15,   15,   15, 0x0a,
      91,   15,   15,   15, 0x0a,
     103,   15,   15,   15, 0x0a,
     120,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_xml_editor[] = {
    "win_xml_editor\0\0file_open_prj()\0"
    "file_open_ssf()\0file_open_pff()\0"
    "file_save()\0file_save_as()\0edit_find()\0"
    "menu_edit_open()\0changed_cursor_pos()\0"
};

const QMetaObject win_xml_editor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_xml_editor,
      qt_meta_data_win_xml_editor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_xml_editor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_xml_editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_xml_editor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_xml_editor))
        return static_cast<void*>(const_cast< win_xml_editor*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_xml_editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: file_open_prj(); break;
        case 1: file_open_ssf(); break;
        case 2: file_open_pff(); break;
        case 3: file_save(); break;
        case 4: file_save_as(); break;
        case 5: edit_find(); break;
        case 6: menu_edit_open(); break;
        case 7: changed_cursor_pos(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
