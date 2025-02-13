/****************************************************************************
** Meta object code from reading C++ file 'lightcontroll.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../lightcontroll.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lightcontroll.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lightcontroll_t {
    QByteArrayData data[38];
    char stringdata0[525];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lightcontroll_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lightcontroll_t qt_meta_stringdata_lightcontroll = {
    {
QT_MOC_LITERAL(0, 0, 13), // "lightcontroll"
QT_MOC_LITERAL(1, 14, 16), // "signalInitWorker"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "showMsg"
QT_MOC_LITERAL(4, 40, 15), // "sigSendDatagram"
QT_MOC_LITERAL(5, 56, 12), // "sendDataList"
QT_MOC_LITERAL(6, 69, 8), // "DeviceId"
QT_MOC_LITERAL(7, 78, 10), // "ContentStr"
QT_MOC_LITERAL(8, 89, 7), // "version"
QT_MOC_LITERAL(9, 97, 9), // "fontColor"
QT_MOC_LITERAL(10, 107, 9), // "Luminance"
QT_MOC_LITERAL(11, 117, 11), // "FlickerList"
QT_MOC_LITERAL(12, 129, 10), // "TermIdSize"
QT_MOC_LITERAL(13, 140, 23), // "siglSetIntervalAndCount"
QT_MOC_LITERAL(14, 164, 17), // "m_sendingInterval"
QT_MOC_LITERAL(15, 182, 14), // "m_sendingCount"
QT_MOC_LITERAL(16, 197, 19), // "sigConnectToControl"
QT_MOC_LITERAL(17, 217, 11), // "write2Kafka"
QT_MOC_LITERAL(18, 229, 5), // "topic"
QT_MOC_LITERAL(19, 235, 7), // "strJson"
QT_MOC_LITERAL(20, 243, 6), // "strKey"
QT_MOC_LITERAL(21, 250, 18), // "signalLightPowerOn"
QT_MOC_LITERAL(22, 269, 2), // "on"
QT_MOC_LITERAL(23, 272, 22), // "signalOpenPathTracking"
QT_MOC_LITERAL(24, 295, 4), // "mode"
QT_MOC_LITERAL(25, 300, 21), // "signalCheckLightState"
QT_MOC_LITERAL(26, 322, 6), // "idlist"
QT_MOC_LITERAL(27, 329, 28), // "signalCheckPathTrackingState"
QT_MOC_LITERAL(28, 358, 18), // "slotReceiveTcpData"
QT_MOC_LITERAL(29, 377, 24), // "slotIsCheckingLightState"
QT_MOC_LITERAL(30, 402, 1), // "b"
QT_MOC_LITERAL(31, 404, 18), // "slotUpdateProgress"
QT_MOC_LITERAL(32, 423, 8), // "progress"
QT_MOC_LITERAL(33, 432, 24), // "slotUpdateLightStateJson"
QT_MOC_LITERAL(34, 457, 11), // "checkIdList"
QT_MOC_LITERAL(35, 469, 18), // "checkResultListTmp"
QT_MOC_LITERAL(36, 488, 31), // "slotUpdatePathTrackingStateJson"
QT_MOC_LITERAL(37, 520, 4) // "time"

    },
    "lightcontroll\0signalInitWorker\0\0showMsg\0"
    "sigSendDatagram\0sendDataList\0DeviceId\0"
    "ContentStr\0version\0fontColor\0Luminance\0"
    "FlickerList\0TermIdSize\0siglSetIntervalAndCount\0"
    "m_sendingInterval\0m_sendingCount\0"
    "sigConnectToControl\0write2Kafka\0topic\0"
    "strJson\0strKey\0signalLightPowerOn\0on\0"
    "signalOpenPathTracking\0mode\0"
    "signalCheckLightState\0idlist\0"
    "signalCheckPathTrackingState\0"
    "slotReceiveTcpData\0slotIsCheckingLightState\0"
    "b\0slotUpdateProgress\0progress\0"
    "slotUpdateLightStateJson\0checkIdList\0"
    "checkResultListTmp\0slotUpdatePathTrackingStateJson\0"
    "time"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lightcontroll[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    1,  100,    2, 0x06 /* Public */,
       4,    8,  103,    2, 0x06 /* Public */,
      13,    2,  120,    2, 0x06 /* Public */,
      13,    1,  125,    2, 0x26 /* Public | MethodCloned */,
      13,    0,  128,    2, 0x26 /* Public | MethodCloned */,
      16,    0,  129,    2, 0x06 /* Public */,
      17,    3,  130,    2, 0x06 /* Public */,
      21,    1,  137,    2, 0x06 /* Public */,
      23,    1,  140,    2, 0x06 /* Public */,
      25,    1,  143,    2, 0x06 /* Public */,
      27,    0,  146,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      28,    1,  147,    2, 0x0a /* Public */,
      29,    1,  150,    2, 0x0a /* Public */,
      31,    1,  153,    2, 0x0a /* Public */,
      33,    2,  156,    2, 0x0a /* Public */,
      36,    2,  161,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::Int,    5,    6,    7,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   18,   19,   20,
    QMetaType::Void, QMetaType::Bool,   22,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, QMetaType::QStringList,   26,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::Bool,   30,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,   34,   35,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   24,   37,

       0        // eod
};

void lightcontroll::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<lightcontroll *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalInitWorker(); break;
        case 1: _t->showMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sigSendDatagram((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 3: _t->siglSetIntervalAndCount((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->siglSetIntervalAndCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->siglSetIntervalAndCount(); break;
        case 6: _t->sigConnectToControl(); break;
        case 7: _t->write2Kafka((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: _t->signalLightPowerOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->signalOpenPathTracking((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->signalCheckLightState((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->signalCheckPathTrackingState(); break;
        case 12: _t->slotReceiveTcpData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 13: _t->slotIsCheckingLightState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->slotUpdateProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->slotUpdateLightStateJson((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 16: _t->slotUpdatePathTrackingStateJson((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (lightcontroll::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::signalInitWorker)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::showMsg)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(QStringList , QString , QString , int , int , int , QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::sigSendDatagram)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::siglSetIntervalAndCount)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::sigConnectToControl)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::write2Kafka)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::signalLightPowerOn)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::signalOpenPathTracking)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::signalCheckLightState)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (lightcontroll::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightcontroll::signalCheckPathTrackingState)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject lightcontroll::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_lightcontroll.data,
    qt_meta_data_lightcontroll,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *lightcontroll::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lightcontroll::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lightcontroll.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int lightcontroll::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void lightcontroll::signalInitWorker()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void lightcontroll::showMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void lightcontroll::sigSendDatagram(QStringList _t1, QString _t2, QString _t3, int _t4, int _t5, int _t6, QString _t7, int _t8)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void lightcontroll::siglSetIntervalAndCount(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 6
void lightcontroll::sigConnectToControl()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void lightcontroll::write2Kafka(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void lightcontroll::signalLightPowerOn(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void lightcontroll::signalOpenPathTracking(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void lightcontroll::signalCheckLightState(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void lightcontroll::signalCheckPathTrackingState()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
