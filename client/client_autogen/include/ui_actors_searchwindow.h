/********************************************************************************
** Form generated from reading UI file 'actors_searchwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTORS_SEARCHWINDOW_H
#define UI_ACTORS_SEARCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Actors_SearchWindow
{
public:

    void setupUi(QWidget *Actors_SearchWindow)
    {
        if (Actors_SearchWindow->objectName().isEmpty())
            Actors_SearchWindow->setObjectName("Actors_SearchWindow");
        Actors_SearchWindow->resize(400, 300);

        retranslateUi(Actors_SearchWindow);

        QMetaObject::connectSlotsByName(Actors_SearchWindow);
    } // setupUi

    void retranslateUi(QWidget *Actors_SearchWindow)
    {
        Actors_SearchWindow->setWindowTitle(QCoreApplication::translate("Actors_SearchWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Actors_SearchWindow: public Ui_Actors_SearchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTORS_SEARCHWINDOW_H
