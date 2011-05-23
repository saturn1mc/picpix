/****************************************************************************
** Meta object code from reading C++ file 'PPWidget.h'
**
** Created: Mon 23. May 19:20:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PPWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PPWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PPWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      19,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PPWidget[] = {
    "PPWidget\0\0update()\0launchAnimation()\0"
};

const QMetaObject PPWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PPWidget,
      qt_meta_data_PPWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PPWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PPWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PPWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PPWidget))
        return static_cast<void*>(const_cast< PPWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PPWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: update(); break;
        case 1: launchAnimation(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
