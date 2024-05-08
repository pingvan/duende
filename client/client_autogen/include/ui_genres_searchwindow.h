/********************************************************************************
** Form generated from reading UI file 'genres_searchwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENRES_SEARCHWINDOW_H
#define UI_GENRES_SEARCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Genres_SearchWindow
{
public:

    void setupUi(QWidget *Genres_SearchWindow)
    {
        if (Genres_SearchWindow->objectName().isEmpty())
            Genres_SearchWindow->setObjectName("Genres_SearchWindow");
        Genres_SearchWindow->resize(400, 300);

        retranslateUi(Genres_SearchWindow);

        QMetaObject::connectSlotsByName(Genres_SearchWindow);
    } // setupUi

    void retranslateUi(QWidget *Genres_SearchWindow)
    {
        Genres_SearchWindow->setWindowTitle(QCoreApplication::translate("Genres_SearchWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Genres_SearchWindow: public Ui_Genres_SearchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENRES_SEARCHWINDOW_H
