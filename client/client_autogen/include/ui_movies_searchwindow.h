/********************************************************************************
** Form generated from reading UI file 'movies_searchwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOVIES_SEARCHWINDOW_H
#define UI_MOVIES_SEARCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Movies_SearchWindow
{
public:

    void setupUi(QWidget *Movies_SearchWindow)
    {
        if (Movies_SearchWindow->objectName().isEmpty())
            Movies_SearchWindow->setObjectName("Movies_SearchWindow");
        Movies_SearchWindow->resize(400, 300);

        retranslateUi(Movies_SearchWindow);

        QMetaObject::connectSlotsByName(Movies_SearchWindow);
    } // setupUi

    void retranslateUi(QWidget *Movies_SearchWindow)
    {
        Movies_SearchWindow->setWindowTitle(QCoreApplication::translate("Movies_SearchWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Movies_SearchWindow: public Ui_Movies_SearchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOVIES_SEARCHWINDOW_H
