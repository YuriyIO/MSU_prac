/********************************************************************************
** Form generated from reading UI file 'secondwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDWINDOW_H
#define UI_SECONDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SecondWindow
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget_requests;
    QLineEdit *lineEdit;
    QListWidget *listWidget_rooms_day_start;
    QLineEdit *lineEdit_2;
    QLabel *label_day_num;
    QLabel *label_income;
    QListWidget *listWidget_rooms_day_end;
    QPushButton *pushButton_continue;
    QPushButton *pushButton_quit;

    void setupUi(QDialog *SecondWindow)
    {
        if (SecondWindow->objectName().isEmpty())
            SecondWindow->setObjectName("SecondWindow");
        SecondWindow->resize(771, 668);
        gridLayout = new QGridLayout(SecondWindow);
        gridLayout->setObjectName("gridLayout");
        listWidget_requests = new QListWidget(SecondWindow);
        listWidget_requests->setObjectName("listWidget_requests");

        gridLayout->addWidget(listWidget_requests, 4, 0, 1, 4);

        lineEdit = new QLineEdit(SecondWindow);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        listWidget_rooms_day_start = new QListWidget(SecondWindow);
        listWidget_rooms_day_start->setObjectName("listWidget_rooms_day_start");

        gridLayout->addWidget(listWidget_rooms_day_start, 3, 0, 1, 4);

        lineEdit_2 = new QLineEdit(SecondWindow);
        lineEdit_2->setObjectName("lineEdit_2");

        gridLayout->addWidget(lineEdit_2, 0, 3, 1, 1);

        label_day_num = new QLabel(SecondWindow);
        label_day_num->setObjectName("label_day_num");

        gridLayout->addWidget(label_day_num, 0, 0, 1, 1);

        label_income = new QLabel(SecondWindow);
        label_income->setObjectName("label_income");

        gridLayout->addWidget(label_income, 0, 2, 1, 1);

        listWidget_rooms_day_end = new QListWidget(SecondWindow);
        listWidget_rooms_day_end->setObjectName("listWidget_rooms_day_end");

        gridLayout->addWidget(listWidget_rooms_day_end, 5, 0, 1, 4);

        pushButton_continue = new QPushButton(SecondWindow);
        pushButton_continue->setObjectName("pushButton_continue");

        gridLayout->addWidget(pushButton_continue, 7, 0, 1, 4);

        pushButton_quit = new QPushButton(SecondWindow);
        pushButton_quit->setObjectName("pushButton_quit");

        gridLayout->addWidget(pushButton_quit, 8, 0, 1, 4);


        retranslateUi(SecondWindow);

        QMetaObject::connectSlotsByName(SecondWindow);
    } // setupUi

    void retranslateUi(QDialog *SecondWindow)
    {
        SecondWindow->setWindowTitle(QCoreApplication::translate("SecondWindow", "Dialog", nullptr));
        label_day_num->setText(QCoreApplication::translate("SecondWindow", "\320\242\320\265\320\272\321\203\321\211\320\270\320\271 \320\264\320\265\320\275\321\214", nullptr));
        label_income->setText(QCoreApplication::translate("SecondWindow", "\320\224\320\276\321\205\320\276\320\264", nullptr));
        pushButton_continue->setText(QCoreApplication::translate("SecondWindow", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214", nullptr));
        pushButton_quit->setText(QCoreApplication::translate("SecondWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SecondWindow: public Ui_SecondWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDWINDOW_H
