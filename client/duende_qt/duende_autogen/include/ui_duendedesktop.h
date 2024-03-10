/********************************************************************************
** Form generated from reading UI file 'duendedesktop.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DUENDEDESKTOP_H
#define UI_DUENDEDESKTOP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_DuendeDesktop
{
public:
    QLabel *label;

    void setupUi(QDialog *DuendeDesktop)
    {
        if (DuendeDesktop->objectName().isEmpty())
            DuendeDesktop->setObjectName(QString::fromUtf8("DuendeDesktop"));
        DuendeDesktop->resize(400, 300);
        label = new QLabel(DuendeDesktop);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 110, 231, 71));

        retranslateUi(DuendeDesktop);

        QMetaObject::connectSlotsByName(DuendeDesktop);
    } // setupUi

    void retranslateUi(QDialog *DuendeDesktop)
    {
        DuendeDesktop->setWindowTitle(QCoreApplication::translate("DuendeDesktop", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DuendeDesktop", "Welcome in Duende!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DuendeDesktop: public Ui_DuendeDesktop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DUENDEDESKTOP_H
