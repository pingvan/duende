/****************************************************************************
** Meta object code from reading C++ file 'manager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/manager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSManagerENDCLASS = QtMocHelpers::stringData(
    "Manager",
    "newMessageReceived",
    "",
    "chat_service::MessageReceived",
    "message",
    "NewInviteReceived",
    "invitation::SendInvitationResponse",
    "response",
    "InviteRespondReceived",
    "invitation::RespondToInvitationResponse"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSManagerENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[8];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[30];
    char stringdata4[8];
    char stringdata5[18];
    char stringdata6[35];
    char stringdata7[9];
    char stringdata8[22];
    char stringdata9[40];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSManagerENDCLASS_t qt_meta_stringdata_CLASSManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 7),  // "Manager"
        QT_MOC_LITERAL(8, 18),  // "newMessageReceived"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 29),  // "chat_service::MessageReceived"
        QT_MOC_LITERAL(58, 7),  // "message"
        QT_MOC_LITERAL(66, 17),  // "NewInviteReceived"
        QT_MOC_LITERAL(84, 34),  // "invitation::SendInvitationRes..."
        QT_MOC_LITERAL(119, 8),  // "response"
        QT_MOC_LITERAL(128, 21),  // "InviteRespondReceived"
        QT_MOC_LITERAL(150, 39)   // "invitation::RespondToInvitati..."
    },
    "Manager",
    "newMessageReceived",
    "",
    "chat_service::MessageReceived",
    "message",
    "NewInviteReceived",
    "invitation::SendInvitationResponse",
    "response",
    "InviteRespondReceived",
    "invitation::RespondToInvitationResponse"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSManagerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       5,    1,   35,    2, 0x06,    3 /* Public */,
       8,    1,   38,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject Manager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Manager, std::true_type>,
        // method 'newMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const chat_service::MessageReceived &, std::false_type>,
        // method 'NewInviteReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const invitation::SendInvitationResponse &, std::false_type>,
        // method 'InviteRespondReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const invitation::RespondToInvitationResponse &, std::false_type>
    >,
    nullptr
} };

void Manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Manager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newMessageReceived((*reinterpret_cast< std::add_pointer_t<chat_service::MessageReceived>>(_a[1]))); break;
        case 1: _t->NewInviteReceived((*reinterpret_cast< std::add_pointer_t<invitation::SendInvitationResponse>>(_a[1]))); break;
        case 2: _t->InviteRespondReceived((*reinterpret_cast< std::add_pointer_t<invitation::RespondToInvitationResponse>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Manager::*)(const chat_service::MessageReceived & );
            if (_t _q_method = &Manager::newMessageReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Manager::*)(const invitation::SendInvitationResponse & );
            if (_t _q_method = &Manager::NewInviteReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Manager::*)(const invitation::RespondToInvitationResponse & );
            if (_t _q_method = &Manager::InviteRespondReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *Manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Manager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Manager::newMessageReceived(const chat_service::MessageReceived & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Manager::NewInviteReceived(const invitation::SendInvitationResponse & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Manager::InviteRespondReceived(const invitation::RespondToInvitationResponse & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
