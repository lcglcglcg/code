/****************************************************************************
** Meta object code from reading C++ file 'baidu.h'
**
** Created: Thu Jul 19 14:37:47 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "baidu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'baidu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebBaidu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   10,    9,    9, 0x0a,
      50,   41,    9,    9, 0x0a,
      77,   68,    9,    9, 0x0a,
     100,   96,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WebBaidu[] = {
    "WebBaidu\0\0reply\0web_pack(QNetworkReply*)\0"
    "progress\0web_progress(int)\0complete\0"
    "web_complete(bool)\0url\0web_open_link(QUrl)\0"
};

void WebBaidu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebBaidu *_t = static_cast<WebBaidu *>(_o);
        switch (_id) {
        case 0: _t->web_pack((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 1: _t->web_progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->web_complete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->web_open_link((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WebBaidu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WebBaidu::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_WebBaidu,
      qt_meta_data_WebBaidu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebBaidu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebBaidu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebBaidu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebBaidu))
        return static_cast<void*>(const_cast< WebBaidu*>(this));
    return QWebView::qt_metacast(_clname);
}

int WebBaidu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
