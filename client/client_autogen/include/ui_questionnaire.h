/********************************************************************************
** Form generated from reading UI file 'questionnaire.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUESTIONNAIRE_H
#define UI_QUESTIONNAIRE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Questionnaire
{
public:

    void setupUi(QWidget *Questionnaire)
    {
        if (Questionnaire->objectName().isEmpty())
            Questionnaire->setObjectName("Questionnaire");
        Questionnaire->resize(956, 644);

        retranslateUi(Questionnaire);

        QMetaObject::connectSlotsByName(Questionnaire);
    } // setupUi

    void retranslateUi(QWidget *Questionnaire)
    {
        Questionnaire->setWindowTitle(QCoreApplication::translate("Questionnaire", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Questionnaire: public Ui_Questionnaire {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUESTIONNAIRE_H
