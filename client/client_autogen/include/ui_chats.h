/********************************************************************************
** Form generated from reading UI file 'chats.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATS_H
#define UI_CHATS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chats
{
public:

    void setupUi(QWidget *Chats)
    {
        if (Chats->objectName().isEmpty())
            Chats->setObjectName("Chats");
        Chats->resize(400, 300);

        retranslateUi(Chats);

        QMetaObject::connectSlotsByName(Chats);
    } // setupUi

    void retranslateUi(QWidget *Chats)
    {
        Chats->setWindowTitle(QCoreApplication::translate("Chats", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chats: public Ui_Chats {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATS_H
