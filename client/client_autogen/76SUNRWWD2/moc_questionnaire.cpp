/****************************************************************************
** Meta object code from reading C++ file 'questionnaire.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/questionnaire/questionnaire.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'questionnaire.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSQuestionnaireENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSQuestionnaireENDCLASS = QtMocHelpers::stringData(
    "Questionnaire",
    "questionnaireFilled",
    "",
    "on_pushButton_save_questionnaire_clicked",
    "onQuestionnaireFilled",
    "updateActorsList",
    "input",
    "updateMoviesList",
    "updateGenresList",
    "onAddActorClicked",
    "onAddMovieClicked",
    "onAddGenreClicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSQuestionnaireENDCLASS_t {
    uint offsetsAndSizes[24];
    char stringdata0[14];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[41];
    char stringdata4[22];
    char stringdata5[17];
    char stringdata6[6];
    char stringdata7[17];
    char stringdata8[17];
    char stringdata9[18];
    char stringdata10[18];
    char stringdata11[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSQuestionnaireENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSQuestionnaireENDCLASS_t qt_meta_stringdata_CLASSQuestionnaireENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "Questionnaire"
        QT_MOC_LITERAL(14, 19),  // "questionnaireFilled"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 40),  // "on_pushButton_save_questionna..."
        QT_MOC_LITERAL(76, 21),  // "onQuestionnaireFilled"
        QT_MOC_LITERAL(98, 16),  // "updateActorsList"
        QT_MOC_LITERAL(115, 5),  // "input"
        QT_MOC_LITERAL(121, 16),  // "updateMoviesList"
        QT_MOC_LITERAL(138, 16),  // "updateGenresList"
        QT_MOC_LITERAL(155, 17),  // "onAddActorClicked"
        QT_MOC_LITERAL(173, 17),  // "onAddMovieClicked"
        QT_MOC_LITERAL(191, 17)   // "onAddGenreClicked"
    },
    "Questionnaire",
    "questionnaireFilled",
    "",
    "on_pushButton_save_questionnaire_clicked",
    "onQuestionnaireFilled",
    "updateActorsList",
    "input",
    "updateMoviesList",
    "updateGenresList",
    "onAddActorClicked",
    "onAddMovieClicked",
    "onAddGenreClicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSQuestionnaireENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    0,   70,    2, 0x08,    3 /* Private */,
       5,    1,   71,    2, 0x08,    4 /* Private */,
       7,    1,   74,    2, 0x08,    6 /* Private */,
       8,    1,   77,    2, 0x08,    8 /* Private */,
       9,    0,   80,    2, 0x08,   10 /* Private */,
      10,    0,   81,    2, 0x08,   11 /* Private */,
      11,    0,   82,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Questionnaire::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSQuestionnaireENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSQuestionnaireENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSQuestionnaireENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Questionnaire, std::true_type>,
        // method 'questionnaireFilled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_save_questionnaire_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onQuestionnaireFilled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateActorsList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateMoviesList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateGenresList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onAddActorClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddMovieClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddGenreClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Questionnaire::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Questionnaire *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->questionnaireFilled(); break;
        case 1: _t->on_pushButton_save_questionnaire_clicked(); break;
        case 2: _t->onQuestionnaireFilled(); break;
        case 3: _t->updateActorsList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->updateMoviesList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->updateGenresList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onAddActorClicked(); break;
        case 7: _t->onAddMovieClicked(); break;
        case 8: _t->onAddGenreClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Questionnaire::*)();
            if (_t _q_method = &Questionnaire::questionnaireFilled; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Questionnaire::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Questionnaire::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSQuestionnaireENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Questionnaire::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Questionnaire::questionnaireFilled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
