/****************************************************************************
** Meta object code from reading C++ file 'controllerworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../controllerworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controllerworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControllerWorker_t {
    QByteArrayData data[28];
    char stringdata0[393];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControllerWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControllerWorker_t qt_meta_stringdata_ControllerWorker = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ControllerWorker"
QT_MOC_LITERAL(1, 17, 16), // "signalLightIsOff"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 7), // "lightId"
QT_MOC_LITERAL(4, 43, 17), // "signalWrite2Kafka"
QT_MOC_LITERAL(5, 61, 14), // "slotInitWorker"
QT_MOC_LITERAL(6, 76, 11), // "slotInitTcp"
QT_MOC_LITERAL(7, 88, 2), // "ip"
QT_MOC_LITERAL(8, 91, 4), // "port"
QT_MOC_LITERAL(9, 96, 11), // "slotInitUdp"
QT_MOC_LITERAL(10, 108, 16), // "slotReadyReadTcp"
QT_MOC_LITERAL(11, 125, 16), // "slotReadyReadUdp"
QT_MOC_LITERAL(12, 142, 18), // "slotCmd2Controller"
QT_MOC_LITERAL(13, 161, 7), // "cmdList"
QT_MOC_LITERAL(14, 169, 21), // "slotFlushAutoCheckCmd"
QT_MOC_LITERAL(15, 191, 13), // "cmdCheckState"
QT_MOC_LITERAL(16, 205, 18), // "cmdCheckPowerState"
QT_MOC_LITERAL(17, 224, 25), // "cmdCheckPathTrackingDelay"
QT_MOC_LITERAL(18, 250, 18), // "slotSendControlCmd"
QT_MOC_LITERAL(19, 269, 16), // "slotSendCheckCmd"
QT_MOC_LITERAL(20, 286, 16), // "slotLightPowerOn"
QT_MOC_LITERAL(21, 303, 2), // "on"
QT_MOC_LITERAL(22, 306, 15), // "slotOpenMarquee"
QT_MOC_LITERAL(23, 322, 4), // "open"
QT_MOC_LITERAL(24, 327, 19), // "slotUpdateLisghtIds"
QT_MOC_LITERAL(25, 347, 8), // "lightIds"
QT_MOC_LITERAL(26, 356, 21), // "slotUpdateMarqueeData"
QT_MOC_LITERAL(27, 378, 14) // "hexContentList"

    },
    "ControllerWorker\0signalLightIsOff\0\0"
    "lightId\0signalWrite2Kafka\0slotInitWorker\0"
    "slotInitTcp\0ip\0port\0slotInitUdp\0"
    "slotReadyReadTcp\0slotReadyReadUdp\0"
    "slotCmd2Controller\0cmdList\0"
    "slotFlushAutoCheckCmd\0cmdCheckState\0"
    "cmdCheckPowerState\0cmdCheckPathTrackingDelay\0"
    "slotSendControlCmd\0slotSendCheckCmd\0"
    "slotLightPowerOn\0on\0slotOpenMarquee\0"
    "open\0slotUpdateLisghtIds\0lightIds\0"
    "slotUpdateMarqueeData\0hexContentList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControllerWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    0,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   93,    2, 0x0a /* Public */,
       6,    2,   94,    2, 0x0a /* Public */,
       9,    0,   99,    2, 0x0a /* Public */,
      10,    0,  100,    2, 0x0a /* Public */,
      11,    0,  101,    2, 0x0a /* Public */,
      12,    1,  102,    2, 0x0a /* Public */,
      14,    3,  105,    2, 0x0a /* Public */,
      18,    1,  112,    2, 0x0a /* Public */,
      19,    1,  115,    2, 0x0a /* Public */,
      20,    1,  118,    2, 0x0a /* Public */,
      22,    1,  121,    2, 0x0a /* Public */,
      24,    1,  124,    2, 0x0a /* Public */,
      26,    1,  127,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   13,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList, QMetaType::QStringList,   15,   16,   17,
    QMetaType::Void, QMetaType::QStringList,   13,
    QMetaType::Void, QMetaType::QStringList,   13,
    QMetaType::Void, QMetaType::Bool,   21,
    QMetaType::Void, QMetaType::Bool,   23,
    QMetaType::Void, QMetaType::QStringList,   25,
    QMetaType::Void, QMetaType::QStringList,   27,

       0        // eod
};

void ControllerWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControllerWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalLightIsOff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signalWrite2Kafka(); break;
        case 2: _t->slotInitWorker(); break;
        case 3: _t->slotInitTcp((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->slotInitUdp(); break;
        case 5: _t->slotReadyReadTcp(); break;
        case 6: _t->slotReadyReadUdp(); break;
        case 7: _t->slotCmd2Controller((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 8: _t->slotFlushAutoCheckCmd((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3]))); break;
        case 9: _t->slotSendControlCmd((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->slotSendCheckCmd((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->slotLightPowerOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->slotOpenMarquee((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->slotUpdateLisghtIds((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 14: _t->slotUpdateMarqueeData((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControllerWorker::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerWorker::signalLightIsOff)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControllerWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerWorker::signalWrite2Kafka)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControllerWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ControllerWorker.data,
    qt_meta_data_ControllerWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControllerWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControllerWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ControllerWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void ControllerWorker::signalLightIsOff(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControllerWorker::signalWrite2Kafka()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
