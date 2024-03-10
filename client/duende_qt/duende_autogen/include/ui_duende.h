/********************************************************************************
** Form generated from reading UI file 'duende.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DUENDE_H
#define UI_DUENDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Duende
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_username;
    QLabel *label_login;
    QLineEdit *lineEdit_login;
    QHBoxLayout *horizontalLayout_password;
    QLabel *label_password;
    QLineEdit *lineEdit_password;
    QPushButton *pushButton_login;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Duende)
    {
        if (Duende->objectName().isEmpty())
            Duende->setObjectName(QString::fromUtf8("Duende"));
        Duende->resize(767, 547);
        centralwidget = new QWidget(Duende);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(200, 140, 351, 271));
        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 40, 315, 211));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_username = new QHBoxLayout();
        horizontalLayout_username->setObjectName(QString::fromUtf8("horizontalLayout_username"));
        label_login = new QLabel(widget);
        label_login->setObjectName(QString::fromUtf8("label_login"));

        horizontalLayout_username->addWidget(label_login);

        lineEdit_login = new QLineEdit(widget);
        lineEdit_login->setObjectName(QString::fromUtf8("lineEdit_login"));

        horizontalLayout_username->addWidget(lineEdit_login);


        verticalLayout->addLayout(horizontalLayout_username);

        horizontalLayout_password = new QHBoxLayout();
        horizontalLayout_password->setObjectName(QString::fromUtf8("horizontalLayout_password"));
        label_password = new QLabel(widget);
        label_password->setObjectName(QString::fromUtf8("label_password"));

        horizontalLayout_password->addWidget(label_password);

        lineEdit_password = new QLineEdit(widget);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));

        horizontalLayout_password->addWidget(lineEdit_password);


        verticalLayout->addLayout(horizontalLayout_password);

        pushButton_login = new QPushButton(widget);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));

        verticalLayout->addWidget(pushButton_login);

        Duende->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Duende);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 767, 26));
        Duende->setMenuBar(menubar);
        statusbar = new QStatusBar(Duende);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Duende->setStatusBar(statusbar);

        retranslateUi(Duende);

        QMetaObject::connectSlotsByName(Duende);
    } // setupUi

    void retranslateUi(QMainWindow *Duende)
    {
        Duende->setWindowTitle(QCoreApplication::translate("Duende", "Duende", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Duende", "Log In", nullptr));
        label_login->setText(QCoreApplication::translate("Duende", "Login", nullptr));
        label_password->setText(QCoreApplication::translate("Duende", "Password", nullptr));
        pushButton_login->setText(QCoreApplication::translate("Duende", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Duende: public Ui_Duende {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DUENDE_H
