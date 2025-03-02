/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_quit;
    QPushButton *pushButton_continue;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QListWidget *listWidget_hotel;
    QLabel *label_10;
    QListWidget *listWidget_req;
    QLabel *label_4;
    QLabel *label_income;
    QLabel *label_cur_day;
    QPushButton *pushButton_to_end;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_day_num;
    QLabel *label_3;
    QLabel *label_5;
    QLineEdit *lineEdit_n5;
    QLabel *label_14;
    QLabel *label_7;
    QLineEdit *lineEdit_p3;
    QLineEdit *lineEdit_n1;
    QLineEdit *lineEdit_p5;
    QLabel *label_8;
    QLineEdit *lineEdit_p4;
    QLineEdit *lineEdit_p2;
    QLabel *label_6;
    QLabel *label_13;
    QLineEdit *lineEdit_n2;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label;
    QLineEdit *lineEdit_room_num;
    QLabel *label_15;
    QLabel *label_2;
    QLineEdit *lineEdit_p1;
    QLineEdit *lineEdit_n3;
    QLineEdit *lineEdit_n4;
    QLineEdit *lineEdit_req_num;
    QLabel *label_9;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1158, 577);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        pushButton_quit = new QPushButton(centralwidget);
        pushButton_quit->setObjectName("pushButton_quit");

        gridLayout->addWidget(pushButton_quit, 11, 0, 1, 8);

        pushButton_continue = new QPushButton(centralwidget);
        pushButton_continue->setObjectName("pushButton_continue");

        gridLayout->addWidget(pushButton_continue, 9, 0, 1, 8);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName("gridLayout_3");
        listWidget_hotel = new QListWidget(groupBox_2);
        listWidget_hotel->setObjectName("listWidget_hotel");

        gridLayout_3->addWidget(listWidget_hotel, 1, 2, 1, 5);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName("label_10");

        gridLayout_3->addWidget(label_10, 0, 4, 1, 1);

        listWidget_req = new QListWidget(groupBox_2);
        listWidget_req->setObjectName("listWidget_req");

        gridLayout_3->addWidget(listWidget_req, 2, 2, 1, 5);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");

        gridLayout_3->addWidget(label_4, 0, 2, 1, 1);

        label_income = new QLabel(groupBox_2);
        label_income->setObjectName("label_income");

        gridLayout_3->addWidget(label_income, 0, 5, 1, 1);

        label_cur_day = new QLabel(groupBox_2);
        label_cur_day->setObjectName("label_cur_day");

        gridLayout_3->addWidget(label_cur_day, 0, 3, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 2, 2, 6);

        pushButton_to_end = new QPushButton(centralwidget);
        pushButton_to_end->setObjectName("pushButton_to_end");

        gridLayout->addWidget(pushButton_to_end, 10, 0, 1, 8);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName("gridLayout_2");
        lineEdit_day_num = new QLineEdit(groupBox);
        lineEdit_day_num->setObjectName("lineEdit_day_num");

        gridLayout_2->addWidget(lineEdit_day_num, 0, 4, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 0, 3, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");

        gridLayout_2->addWidget(label_5, 2, 1, 1, 1);

        lineEdit_n5 = new QLineEdit(groupBox);
        lineEdit_n5->setObjectName("lineEdit_n5");

        gridLayout_2->addWidget(lineEdit_n5, 5, 2, 1, 1);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName("label_14");

        gridLayout_2->addWidget(label_14, 4, 3, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");

        gridLayout_2->addWidget(label_7, 4, 1, 1, 1);

        lineEdit_p3 = new QLineEdit(groupBox);
        lineEdit_p3->setObjectName("lineEdit_p3");

        gridLayout_2->addWidget(lineEdit_p3, 3, 4, 1, 1);

        lineEdit_n1 = new QLineEdit(groupBox);
        lineEdit_n1->setObjectName("lineEdit_n1");

        gridLayout_2->addWidget(lineEdit_n1, 1, 2, 1, 1);

        lineEdit_p5 = new QLineEdit(groupBox);
        lineEdit_p5->setObjectName("lineEdit_p5");

        gridLayout_2->addWidget(lineEdit_p5, 5, 4, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");

        gridLayout_2->addWidget(label_8, 5, 1, 1, 1);

        lineEdit_p4 = new QLineEdit(groupBox);
        lineEdit_p4->setObjectName("lineEdit_p4");

        gridLayout_2->addWidget(lineEdit_p4, 4, 4, 1, 1);

        lineEdit_p2 = new QLineEdit(groupBox);
        lineEdit_p2->setObjectName("lineEdit_p2");

        gridLayout_2->addWidget(lineEdit_p2, 2, 4, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");

        gridLayout_2->addWidget(label_6, 3, 1, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName("label_13");

        gridLayout_2->addWidget(label_13, 3, 3, 1, 1);

        lineEdit_n2 = new QLineEdit(groupBox);
        lineEdit_n2->setObjectName("lineEdit_n2");

        gridLayout_2->addWidget(lineEdit_n2, 2, 2, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName("label_11");

        gridLayout_2->addWidget(label_11, 1, 3, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName("label_12");

        gridLayout_2->addWidget(label_12, 2, 3, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        lineEdit_room_num = new QLineEdit(groupBox);
        lineEdit_room_num->setObjectName("lineEdit_room_num");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(lineEdit_room_num->sizePolicy().hasHeightForWidth());
        lineEdit_room_num->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(lineEdit_room_num, 0, 2, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName("label_15");

        gridLayout_2->addWidget(label_15, 5, 3, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 1, 1, 1, 1);

        lineEdit_p1 = new QLineEdit(groupBox);
        lineEdit_p1->setObjectName("lineEdit_p1");

        gridLayout_2->addWidget(lineEdit_p1, 1, 4, 1, 1);

        lineEdit_n3 = new QLineEdit(groupBox);
        lineEdit_n3->setObjectName("lineEdit_n3");

        gridLayout_2->addWidget(lineEdit_n3, 3, 2, 1, 1);

        lineEdit_n4 = new QLineEdit(groupBox);
        lineEdit_n4->setObjectName("lineEdit_n4");

        gridLayout_2->addWidget(lineEdit_n4, 4, 2, 1, 1);

        lineEdit_req_num = new QLineEdit(groupBox);
        lineEdit_req_num->setObjectName("lineEdit_req_num");

        gridLayout_2->addWidget(lineEdit_req_num, 6, 2, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");

        gridLayout_2->addWidget(label_9, 6, 1, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 2, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1158, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_quit->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        pushButton_continue->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\321\205\320\276\320\264", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\274\320\265\321\200 \320\264\320\275\321\217", nullptr));
        label_income->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_cur_day->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButton_to_end->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\264\320\276 \320\272\320\276\320\275\321\206\320\260", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        lineEdit_day_num->setText(QCoreApplication::translate("MainWindow", "12", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\264\320\275\320\265\320\271(12-30)", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\224\320\262\321\203\320\274\320\265\321\201\321\202\320\275\321\213\320\265", nullptr));
        lineEdit_n5->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "                               \320\246\320\265\320\275\320\260", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\321\203\320\273\321\216\320\272\321\201", nullptr));
        lineEdit_p3->setText(QCoreApplication::translate("MainWindow", "110", nullptr));
        lineEdit_n1->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        lineEdit_p5->setText(QCoreApplication::translate("MainWindow", "150", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\320\233\321\216\320\272\321\201", nullptr));
        lineEdit_p4->setText(QCoreApplication::translate("MainWindow", "130", nullptr));
        lineEdit_p2->setText(QCoreApplication::translate("MainWindow", "90", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\221\320\276\320\273\321\214\321\210\320\270\320\265 \320\264\320\262\321\203\320\274\320\265\321\201\321\202\320\275\321\213\320\265", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "                               \320\246\320\265\320\275\320\260", nullptr));
        lineEdit_n2->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "                               \320\246\320\265\320\275\320\260", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "                               \320\246\320\265\320\275\320\260", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\272\320\276\320\274\320\275\320\260\321\202(20-30)", nullptr));
        lineEdit_room_num->setText(QCoreApplication::translate("MainWindow", "28", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "                               \320\246\320\265\320\275\320\260", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\236\320\264\320\275\320\276\320\274\320\265\321\201\321\202\320\275\321\213\320\265", nullptr));
        lineEdit_p1->setText(QCoreApplication::translate("MainWindow", "70", nullptr));
        lineEdit_n3->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        lineEdit_n4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        lineEdit_req_num->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\320\272\321\201 \321\207\320\270\321\201\320\273\320\276 \320\267\320\260\321\217\320\262\320\276\320\272", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
