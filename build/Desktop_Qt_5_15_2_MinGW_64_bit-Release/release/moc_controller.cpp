/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Controller_t {
    QByteArrayData data[23];
    char stringdata0[267];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Controller_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Controller_t qt_meta_stringdata_Controller = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Controller"
QT_MOC_LITERAL(1, 11, 16), // "signalInitWorker"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "signalInitTcp"
QT_MOC_LITERAL(4, 43, 2), // "ip"
QT_MOC_LITERAL(5, 46, 4), // "port"
QT_MOC_LITERAL(6, 51, 13), // "signalInitUdp"
QT_MOC_LITERAL(7, 65, 23), // "signalFlushAutoCheckCmd"
QT_MOC_LITERAL(8, 89, 9), // "cmdCheck1"
QT_MOC_LITERAL(9, 99, 9), // "cmdCheck2"
QT_MOC_LITERAL(10, 109, 9), // "cmdCheck3"
QT_MOC_LITERAL(11, 119, 20), // "signalSendControlCmd"
QT_MOC_LITERAL(12, 140, 7), // "cmdList"
QT_MOC_LITERAL(13, 148, 18), // "signalSendCheckCmd"
QT_MOC_LITERAL(14, 167, 18), // "signalLightPowerOn"
QT_MOC_LITERAL(15, 186, 2), // "on"
QT_MOC_LITERAL(16, 189, 17), // "signalWrite2Kafka"
QT_MOC_LITERAL(17, 207, 5), // "topic"
QT_MOC_LITERAL(18, 213, 7), // "strJson"
QT_MOC_LITERAL(19, 221, 6), // "strKey"
QT_MOC_LITERAL(20, 228, 14), // "slotLightIsOff"
QT_MOC_LITERAL(21, 243, 7), // "lightId"
QT_MOC_LITERAL(22, 251, 15) // "slotWrite2Kafka"

    },
    "Controller\0signalInitWorker\0\0signalInitTcp\0"
    "ip\0port\0signalInitUdp\0signalFlushAutoCheckCmd\0"
    "cmdCheck1\0cmdCheck2\0cmdCheck3\0"
    "signalSendControlCmd\0cmdList\0"
    "signalSendCheckCmd\0signalLightPowerOn\0"
    "on\0signalWrite2Kafka\0topic\0strJson\0"
    "strKey\0slotLightIsOff\0lightId\0"
    "slotWrite2Kafka"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    2,   65,    2, 0x06 /* Public */,
       6,    0,   70,    2, 0x06 /* Public */,
       7,    3,   71,    2, 0x06 /* Public */,
      11,    1,   78,    2, 0x06 /* Public */,
      13,    1,   81,    2, 0x06 /* Public */,
      14,    1,   84,    2, 0x06 /* Public */,
      16,    3,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    1,   94,    2, 0x0a /* Public */,
      22,    0,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList, QMetaType::QStringList,    8,    9,   10,
    QMetaType::Void, QMetaType::QStringList,   12,
    QMetaType::Void, QMetaType::QStringList,   12,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   18,   19,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,

       0        // eod
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Controller *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalInitWorker(); break;
        case 1: _t->signalInitTcp((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->signalInitUdp(); break;
        case 3: _t->signalFlushAutoCheckCmd((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3]))); break;
        case 4: _t->signalSendControlCmd((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 5: _t->signalSendCheckCmd((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 6: _t->signalLightPowerOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->signalWrite2Kafka((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: _t->slotLightIsOff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->slotWrite2Kafka(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Controller::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalInitWorker)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalInitTcp)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Controller::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalInitUdp)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QStringList , QStringList , QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalFlushAutoCheckCmd)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalSendControlCmd)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalSendCheckCmd)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Controller::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalLightPowerOn)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Controller::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::signalWrite2Kafka)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Controller::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Controller.data,
    qt_meta_data_Controller,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Controller.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Controller::signalInitWorker()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Controller::signalInitTcp(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Controller::signalInitUdp()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Controller::signalFlushAutoCheckCmd(QStringList _t1, QStringList _t2, QStringList _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Controller::signalSendControlCmd(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Controller::signalSendCheckCmd(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Controller::signalLightPowerOn(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Controller::signalWrite2Kafka(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
