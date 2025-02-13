/****************************************************************************
** Meta object code from reading C++ file 'lightcontrollworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../lightcontrollworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lightcontrollworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lightControllWorker_t {
    QByteArrayData data[44];
    char stringdata0[603];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lightControllWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lightControllWorker_t qt_meta_stringdata_lightControllWorker = {
    {
QT_MOC_LITERAL(0, 0, 19), // "lightControllWorker"
QT_MOC_LITERAL(1, 20, 7), // "showMsg"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "sigReceiveData"
QT_MOC_LITERAL(4, 44, 11), // "write2Kafka"
QT_MOC_LITERAL(5, 56, 5), // "topic"
QT_MOC_LITERAL(6, 62, 7), // "strJson"
QT_MOC_LITERAL(7, 70, 6), // "strKey"
QT_MOC_LITERAL(8, 77, 28), // "signalFinishUpdateLightState"
QT_MOC_LITERAL(9, 106, 26), // "signalIsCheckingLightState"
QT_MOC_LITERAL(10, 133, 1), // "b"
QT_MOC_LITERAL(11, 135, 20), // "signalUpdateProgress"
QT_MOC_LITERAL(12, 156, 8), // "progress"
QT_MOC_LITERAL(13, 165, 10), // "signalTest"
QT_MOC_LITERAL(14, 176, 2), // "ba"
QT_MOC_LITERAL(15, 179, 26), // "signalUpdateLightStateJson"
QT_MOC_LITERAL(16, 206, 11), // "checkIdList"
QT_MOC_LITERAL(17, 218, 18), // "checkResultListTmp"
QT_MOC_LITERAL(18, 237, 33), // "signalUpdatePathTrackingState..."
QT_MOC_LITERAL(19, 271, 4), // "mode"
QT_MOC_LITERAL(20, 276, 4), // "time"
QT_MOC_LITERAL(21, 281, 8), // "slotInit"
QT_MOC_LITERAL(22, 290, 16), // "slotSendDatagram"
QT_MOC_LITERAL(23, 307, 12), // "sendDataList"
QT_MOC_LITERAL(24, 320, 8), // "DeviceId"
QT_MOC_LITERAL(25, 329, 10), // "ContentStr"
QT_MOC_LITERAL(26, 340, 7), // "version"
QT_MOC_LITERAL(27, 348, 9), // "fontColor"
QT_MOC_LITERAL(28, 358, 9), // "Luminance"
QT_MOC_LITERAL(29, 368, 11), // "FlickerList"
QT_MOC_LITERAL(30, 380, 10), // "TermIdSize"
QT_MOC_LITERAL(31, 391, 16), // "slotReadyReadTcp"
QT_MOC_LITERAL(32, 408, 16), // "slotReadyReadUdp"
QT_MOC_LITERAL(33, 425, 23), // "slotSetIntervalAndCount"
QT_MOC_LITERAL(34, 449, 15), // "sendingInterval"
QT_MOC_LITERAL(35, 465, 12), // "sendingCount"
QT_MOC_LITERAL(36, 478, 20), // "slotConnectToControl"
QT_MOC_LITERAL(37, 499, 16), // "slotLightPowerOn"
QT_MOC_LITERAL(38, 516, 2), // "on"
QT_MOC_LITERAL(39, 519, 20), // "slotOpenPathTracking"
QT_MOC_LITERAL(40, 540, 19), // "slotCheckLightState"
QT_MOC_LITERAL(41, 560, 6), // "idlist"
QT_MOC_LITERAL(42, 567, 26), // "slotCheckPathTrackingState"
QT_MOC_LITERAL(43, 594, 8) // "slotTest"

    },
    "lightControllWorker\0showMsg\0\0"
    "sigReceiveData\0write2Kafka\0topic\0"
    "strJson\0strKey\0signalFinishUpdateLightState\0"
    "signalIsCheckingLightState\0b\0"
    "signalUpdateProgress\0progress\0signalTest\0"
    "ba\0signalUpdateLightStateJson\0checkIdList\0"
    "checkResultListTmp\0signalUpdatePathTrackingStateJson\0"
    "mode\0time\0slotInit\0slotSendDatagram\0"
    "sendDataList\0DeviceId\0ContentStr\0"
    "version\0fontColor\0Luminance\0FlickerList\0"
    "TermIdSize\0slotReadyReadTcp\0"
    "slotReadyReadUdp\0slotSetIntervalAndCount\0"
    "sendingInterval\0sendingCount\0"
    "slotConnectToControl\0slotLightPowerOn\0"
    "on\0slotOpenPathTracking\0slotCheckLightState\0"
    "idlist\0slotCheckPathTrackingState\0"
    "slotTest"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lightControllWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       3,    1,  117,    2, 0x06 /* Public */,
       4,    3,  120,    2, 0x06 /* Public */,
       8,    0,  127,    2, 0x06 /* Public */,
       9,    1,  128,    2, 0x06 /* Public */,
      11,    1,  131,    2, 0x06 /* Public */,
      13,    1,  134,    2, 0x06 /* Public */,
      15,    2,  137,    2, 0x06 /* Public */,
      18,    2,  142,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    0,  147,    2, 0x0a /* Public */,
      22,    8,  148,    2, 0x0a /* Public */,
      31,    0,  165,    2, 0x0a /* Public */,
      32,    0,  166,    2, 0x0a /* Public */,
      33,    2,  167,    2, 0x0a /* Public */,
      36,    0,  172,    2, 0x0a /* Public */,
      37,    1,  173,    2, 0x0a /* Public */,
      39,    1,  176,    2, 0x0a /* Public */,
      40,    1,  179,    2, 0x0a /* Public */,
      42,    0,  182,    2, 0x0a /* Public */,
      43,    1,  183,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QByteArray,   14,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,   16,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   19,   20,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::Int,   23,   24,   25,   26,   27,   28,   29,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   34,   35,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   38,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::QStringList,   41,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   14,

       0        // eod
};

void lightControllWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<lightControllWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sigReceiveData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->write2Kafka((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->signalFinishUpdateLightState(); break;
        case 4: _t->signalIsCheckingLightState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->signalUpdateProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->signalTest((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->signalUpdateLightStateJson((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 8: _t->signalUpdatePathTrackingStateJson((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->slotInit(); break;
        case 10: _t->slotSendDatagram((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 11: _t->slotReadyReadTcp(); break;
        case 12: _t->slotReadyReadUdp(); break;
        case 13: _t->slotSetIntervalAndCount((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->slotConnectToControl(); break;
        case 15: _t->slotLightPowerOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->slotOpenPathTracking((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->slotCheckLightState((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 18: _t->slotCheckPathTrackingState(); break;
        case 19: _t->slotTest((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (lightControllWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::showMsg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::sigReceiveData)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::write2Kafka)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::signalFinishUpdateLightState)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::signalIsCheckingLightState)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::signalUpdateProgress)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::signalTest)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(QStringList , QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::signalUpdateLightStateJson)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (lightControllWorker::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lightControllWorker::signalUpdatePathTrackingStateJson)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject lightControllWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_lightControllWorker.data,
    qt_meta_data_lightControllWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *lightControllWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lightControllWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lightControllWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int lightControllWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void lightControllWorker::showMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void lightControllWorker::sigReceiveData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void lightControllWorker::write2Kafka(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void lightControllWorker::signalFinishUpdateLightState()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void lightControllWorker::signalIsCheckingLightState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void lightControllWorker::signalUpdateProgress(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void lightControllWorker::signalTest(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void lightControllWorker::signalUpdateLightStateJson(QStringList _t1, QStringList _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void lightControllWorker::signalUpdatePathTrackingStateJson(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
