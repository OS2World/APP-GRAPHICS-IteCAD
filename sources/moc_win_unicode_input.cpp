/****************************************************************************
** Meta object code from reading C++ file 'win_unicode_input.h'
**
** Created: Sun Dec 18 11:14:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "win_unicode_input.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'win_unicode_input.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CharacterWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CharacterWidget[] = {
    "CharacterWidget\0\0character\0"
    "characterSelected(QString)\0"
};

const QMetaObject CharacterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CharacterWidget,
      qt_meta_data_CharacterWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CharacterWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CharacterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CharacterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CharacterWidget))
        return static_cast<void*>(const_cast< CharacterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CharacterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: characterSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CharacterWidget::characterSelected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_win_unicode_input[] = {

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
      27,   19,   18,   18, 0x0a,
      48,   19,   18,   18, 0x0a,
      70,   67,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_win_unicode_input[] = {
    "win_unicode_input\0\0inutile\0"
    "set_electronic(bool)\0set_accented(bool)\0"
    "le\0set_lineedit(QLineEdit*)\0"
};

const QMetaObject win_unicode_input::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_win_unicode_input,
      qt_meta_data_win_unicode_input, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &win_unicode_input::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *win_unicode_input::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *win_unicode_input::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_win_unicode_input))
        return static_cast<void*>(const_cast< win_unicode_input*>(this));
    return QDialog::qt_metacast(_clname);
}

int win_unicode_input::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: set_electronic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: set_accented((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: set_lineedit((*reinterpret_cast< QLineEdit*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
