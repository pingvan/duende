/********************************************************************************
** Form generated from reading UI file 'recommendations.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECOMMENDATIONS_H
#define UI_RECOMMENDATIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Recommendations
{
public:

    void setupUi(QWidget *Recommendations)
    {
        if (Recommendations->objectName().isEmpty())
            Recommendations->setObjectName("Recommendations");
        Recommendations->resize(877, 614);

        retranslateUi(Recommendations);

        QMetaObject::connectSlotsByName(Recommendations);
    } // setupUi

    void retranslateUi(QWidget *Recommendations)
    {
        Recommendations->setWindowTitle(QCoreApplication::translate("Recommendations", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Recommendations: public Ui_Recommendations {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECOMMENDATIONS_H
