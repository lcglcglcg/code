/****************************************************************************
** Meta object code from reading C++ file 'keyword.h'
**
** Created: Mon Jul 30 13:33:05 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "keyword.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keyword.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KeyWord[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   30,    8,    8, 0x0a,
      70,   61,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KeyWord[] = {
    "KeyWord\0\0signal_keyword_end()\0progress\0"
    "keyword_progress(int)\0complete\0"
    "keyword_complete(bool)\0"
};

void KeyWord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KeyWord *_t = static_cast<KeyWord *>(_o);
        switch (_id) {
        case 0: _t->signal_keyword_end(); break;
        case 1: _t->keyword_progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->keyword_complete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData KeyWord::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject KeyWord::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_KeyWord,
      qt_meta_data_KeyWord, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KeyWord::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KeyWord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KeyWord::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KeyWord))
        return static_cast<void*>(const_cast< KeyWord*>(this));
    return QWebView::qt_metacast(_clname);
}

int KeyWord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void KeyWord::signal_keyword_end()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
