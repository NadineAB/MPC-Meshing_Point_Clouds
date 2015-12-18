/****************************************************************************
** Meta object code from reading C++ file 'glCanvas.h'
**
** Created: Fri 6. Sep 00:13:13 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../glCanvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glCanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GLCanvas[] = {

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
      13,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,    9,    9,    9, 0x0a,
      56,   49,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GLCanvas[] = {
    "GLCanvas\0\0ns\0statusBarMessage(QString)\0"
    "animate()\0enable\0setDisplayInfo(bool)\0"
};

void GLCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GLCanvas *_t = static_cast<GLCanvas *>(_o);
        switch (_id) {
        case 0: _t->statusBarMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->animate(); break;
        case 2: _t->setDisplayInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GLCanvas::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GLCanvas::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLCanvas,
      qt_meta_data_GLCanvas, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GLCanvas::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GLCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GLCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLCanvas))
        return static_cast<void*>(const_cast< GLCanvas*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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
void GLCanvas::statusBarMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
