/****************************************************************************
** Meta object code from reading C++ file 'chats.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/chats/chats.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chats.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSChatsENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSChatsENDCLASS = QtMocHelpers::stringData(
    "Chats",
    "messageReceived",
    "",
    "message",
    "build",
    "connect_slots",
    "onChatClicked",
    "QListWidgetItem*",
    "item",
    "onInviteClicked",
    "sendMessage",
    "onNewMessageReceived",
    "chat_service::MessageReceived",
    "onNewInviteReceived",
    "invitation::SendInvitationResponse",
    "invite_response",
    "onInviteRespond",
    "accept",
    "onInviteRespondReceived",
    "invitation::RespondToInvitationResponse",
    "make_chat_item",
    "chat_service::ChatPreview",
    "chat",
    "chat_page_index",
    "item_index",
    "make_message_item",
    "make_invite_item",
    "invitation::Invite",
    "invite",
    "am_i_sender",
    "delete_invite_item",
    "invite_id",
    "updateChatList",
    "input",
    "updateInviteList"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSChatsENDCLASS_t {
    uint offsetsAndSizes[70];
    char stringdata0[6];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[6];
    char stringdata5[14];
    char stringdata6[14];
    char stringdata7[17];
    char stringdata8[5];
    char stringdata9[16];
    char stringdata10[12];
    char stringdata11[21];
    char stringdata12[30];
    char stringdata13[20];
    char stringdata14[35];
    char stringdata15[16];
    char stringdata16[16];
    char stringdata17[7];
    char stringdata18[24];
    char stringdata19[40];
    char stringdata20[15];
    char stringdata21[26];
    char stringdata22[5];
    char stringdata23[16];
    char stringdata24[11];
    char stringdata25[18];
    char stringdata26[17];
    char stringdata27[19];
    char stringdata28[7];
    char stringdata29[12];
    char stringdata30[19];
    char stringdata31[10];
    char stringdata32[15];
    char stringdata33[6];
    char stringdata34[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSChatsENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSChatsENDCLASS_t qt_meta_stringdata_CLASSChatsENDCLASS = {
    {
        QT_MOC_LITERAL(0, 5),  // "Chats"
        QT_MOC_LITERAL(6, 15),  // "messageReceived"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 7),  // "message"
        QT_MOC_LITERAL(31, 5),  // "build"
        QT_MOC_LITERAL(37, 13),  // "connect_slots"
        QT_MOC_LITERAL(51, 13),  // "onChatClicked"
        QT_MOC_LITERAL(65, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(82, 4),  // "item"
        QT_MOC_LITERAL(87, 15),  // "onInviteClicked"
        QT_MOC_LITERAL(103, 11),  // "sendMessage"
        QT_MOC_LITERAL(115, 20),  // "onNewMessageReceived"
        QT_MOC_LITERAL(136, 29),  // "chat_service::MessageReceived"
        QT_MOC_LITERAL(166, 19),  // "onNewInviteReceived"
        QT_MOC_LITERAL(186, 34),  // "invitation::SendInvitationRes..."
        QT_MOC_LITERAL(221, 15),  // "invite_response"
        QT_MOC_LITERAL(237, 15),  // "onInviteRespond"
        QT_MOC_LITERAL(253, 6),  // "accept"
        QT_MOC_LITERAL(260, 23),  // "onInviteRespondReceived"
        QT_MOC_LITERAL(284, 39),  // "invitation::RespondToInvitati..."
        QT_MOC_LITERAL(324, 14),  // "make_chat_item"
        QT_MOC_LITERAL(339, 25),  // "chat_service::ChatPreview"
        QT_MOC_LITERAL(365, 4),  // "chat"
        QT_MOC_LITERAL(370, 15),  // "chat_page_index"
        QT_MOC_LITERAL(386, 10),  // "item_index"
        QT_MOC_LITERAL(397, 17),  // "make_message_item"
        QT_MOC_LITERAL(415, 16),  // "make_invite_item"
        QT_MOC_LITERAL(432, 18),  // "invitation::Invite"
        QT_MOC_LITERAL(451, 6),  // "invite"
        QT_MOC_LITERAL(458, 11),  // "am_i_sender"
        QT_MOC_LITERAL(470, 18),  // "delete_invite_item"
        QT_MOC_LITERAL(489, 9),  // "invite_id"
        QT_MOC_LITERAL(499, 14),  // "updateChatList"
        QT_MOC_LITERAL(514, 5),  // "input"
        QT_MOC_LITERAL(520, 16)   // "updateInviteList"
    },
    "Chats",
    "messageReceived",
    "",
    "message",
    "build",
    "connect_slots",
    "onChatClicked",
    "QListWidgetItem*",
    "item",
    "onInviteClicked",
    "sendMessage",
    "onNewMessageReceived",
    "chat_service::MessageReceived",
    "onNewInviteReceived",
    "invitation::SendInvitationResponse",
    "invite_response",
    "onInviteRespond",
    "accept",
    "onInviteRespondReceived",
    "invitation::RespondToInvitationResponse",
    "make_chat_item",
    "chat_service::ChatPreview",
    "chat",
    "chat_page_index",
    "item_index",
    "make_message_item",
    "make_invite_item",
    "invitation::Invite",
    "invite",
    "am_i_sender",
    "delete_invite_item",
    "invite_id",
    "updateChatList",
    "input",
    "updateInviteList"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSChatsENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  110,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,  113,    2, 0x08,    3 /* Private */,
       5,    0,  114,    2, 0x08,    4 /* Private */,
       6,    1,  115,    2, 0x08,    5 /* Private */,
       9,    1,  118,    2, 0x08,    7 /* Private */,
      10,    0,  121,    2, 0x08,    9 /* Private */,
      11,    1,  122,    2, 0x08,   10 /* Private */,
      13,    1,  125,    2, 0x08,   12 /* Private */,
      16,    1,  128,    2, 0x08,   14 /* Private */,
      18,    1,  131,    2, 0x08,   16 /* Private */,
      20,    3,  134,    2, 0x08,   18 /* Private */,
      25,    1,  141,    2, 0x08,   22 /* Private */,
      26,    2,  144,    2, 0x08,   24 /* Private */,
      30,    1,  149,    2, 0x08,   27 /* Private */,
      32,    1,  152,    2, 0x08,   29 /* Private */,
      34,    1,  155,    2, 0x08,   31 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,    3,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, 0x80000000 | 19,    2,
    QMetaType::Void, 0x80000000 | 21, QMetaType::Int, QMetaType::Int,   22,   23,   24,
    QMetaType::Void, 0x80000000 | 12,    3,
    QMetaType::Void, 0x80000000 | 27, QMetaType::Bool,   28,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QString,   33,

       0        // eod
};

Q_CONSTINIT const QMetaObject Chats::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSChatsENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSChatsENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSChatsENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Chats, std::true_type>,
        // method 'messageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'build'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connect_slots'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onChatClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onInviteClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'sendMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const chat_service::MessageReceived &, std::false_type>,
        // method 'onNewInviteReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const invitation::SendInvitationResponse &, std::false_type>,
        // method 'onInviteRespond'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onInviteRespondReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const invitation::RespondToInvitationResponse &, std::false_type>,
        // method 'make_chat_item'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const chat_service::ChatPreview &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'make_message_item'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const chat_service::MessageReceived &, std::false_type>,
        // method 'make_invite_item'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const invitation::Invite &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'delete_invite_item'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateChatList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateInviteList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void Chats::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Chats *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->build(); break;
        case 2: _t->connect_slots(); break;
        case 3: _t->onChatClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 4: _t->onInviteClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 5: _t->sendMessage(); break;
        case 6: _t->onNewMessageReceived((*reinterpret_cast< std::add_pointer_t<chat_service::MessageReceived>>(_a[1]))); break;
        case 7: _t->onNewInviteReceived((*reinterpret_cast< std::add_pointer_t<invitation::SendInvitationResponse>>(_a[1]))); break;
        case 8: _t->onInviteRespond((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->onInviteRespondReceived((*reinterpret_cast< std::add_pointer_t<invitation::RespondToInvitationResponse>>(_a[1]))); break;
        case 10: _t->make_chat_item((*reinterpret_cast< std::add_pointer_t<chat_service::ChatPreview>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 11: _t->make_message_item((*reinterpret_cast< std::add_pointer_t<chat_service::MessageReceived>>(_a[1]))); break;
        case 12: _t->make_invite_item((*reinterpret_cast< std::add_pointer_t<invitation::Invite>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 13: _t->delete_invite_item((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->updateChatList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->updateInviteList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Chats::*)(const QString & );
            if (_t _q_method = &Chats::messageReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Chats::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chats::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSChatsENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Chats::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void Chats::messageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
