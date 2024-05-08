/********************************************************************************
** Form generated from reading UI file 'follows.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOLLOWS_H
#define UI_FOLLOWS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Follows
{
public:

    void setupUi(QWidget *Follows)
    {
        if (Follows->objectName().isEmpty())
            Follows->setObjectName("Follows");
        Follows->resize(857, 600);

        retranslateUi(Follows);

        QMetaObject::connectSlotsByName(Follows);
    } // setupUi

    void retranslateUi(QWidget *Follows)
    {
        Follows->setWindowTitle(QCoreApplication::translate("Follows", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Follows: public Ui_Follows {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOLLOWS_H
