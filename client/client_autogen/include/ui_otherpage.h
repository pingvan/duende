/********************************************************************************
** Form generated from reading UI file 'otherpage.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OTHERPAGE_H
#define UI_OTHERPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OtherPage
{
public:

    void setupUi(QWidget *OtherPage)
    {
        if (OtherPage->objectName().isEmpty())
            OtherPage->setObjectName("OtherPage");
        OtherPage->resize(400, 300);

        retranslateUi(OtherPage);

        QMetaObject::connectSlotsByName(OtherPage);
    } // setupUi

    void retranslateUi(QWidget *OtherPage)
    {
        OtherPage->setWindowTitle(QCoreApplication::translate("OtherPage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OtherPage: public Ui_OtherPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OTHERPAGE_H
