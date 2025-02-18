/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_RUN;
    QLabel *label_DK;
    QPushButton *pushButton_STOP;
    QGroupBox *groupBox_3;
    QLabel *label_3;
    QPushButton *pushButton_STOP_2;
    QLineEdit *lineEdit;
    QGroupBox *groupBox_6;
    QTextBrowser *textBrowser_2;
    QGroupBox *groupBox_5;
    QTextBrowser *textBrowser;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *customPlotWidget;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1205, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 20, 541, 531));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(false);
        font.setStrikeOut(false);
        font.setKerning(true);
        groupBox->setFont(font);
        groupBox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        groupBox->setAutoFillBackground(false);
        groupBox->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 310, 261, 211));
        QFont font1;
        font1.setPointSize(13);
        font1.setBold(true);
        groupBox_2->setFont(font1);
        groupBox_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(170, 255, 255);"));
        pushButton_RUN = new QPushButton(groupBox_2);
        pushButton_RUN->setObjectName("pushButton_RUN");
        pushButton_RUN->setGeometry(QRect(10, 120, 111, 41));
        QFont font2;
        font2.setPointSize(16);
        font2.setBold(true);
        pushButton_RUN->setFont(font2);
        pushButton_RUN->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(85, 255, 127);"));
        label_DK = new QLabel(groupBox_2);
        label_DK->setObjectName("label_DK");
        label_DK->setGeometry(QRect(50, 40, 161, 41));
        label_DK->setFont(font2);
        label_DK->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 255, 127);"));
        pushButton_STOP = new QPushButton(groupBox_2);
        pushButton_STOP->setObjectName("pushButton_STOP");
        pushButton_STOP->setGeometry(QRect(140, 120, 111, 41));
        pushButton_STOP->setFont(font2);
        pushButton_STOP->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"color: rgb(0, 0, 0);"));
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(270, 310, 261, 211));
        groupBox_3->setFont(font1);
        groupBox_3->setTabletTracking(false);
        groupBox_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(170, 255, 255);"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 50, 91, 16));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        pushButton_STOP_2 = new QPushButton(groupBox_3);
        pushButton_STOP_2->setObjectName("pushButton_STOP_2");
        pushButton_STOP_2->setGeometry(QRect(80, 120, 111, 41));
        pushButton_STOP_2->setFont(font2);
        pushButton_STOP_2->setStyleSheet(QString::fromUtf8("border-color: rgb(255, 0, 0);\n"
"selection-background-color: rgb(255, 255, 255);\n"
"border-top-color: rgb(255, 85, 127);\n"
"alternate-background-color: rgb(85, 85, 0);\n"
"background-color: rgb(255, 170, 0);\n"
"color: rgb(255, 0, 0);\n"
"gridline-color: rgb(255, 0, 0);\n"
"selection-color: rgb(255, 0, 0);"));
        lineEdit = new QLineEdit(groupBox_3);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(120, 40, 113, 41));
        lineEdit->setFont(font2);
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 127);"));
        groupBox_6 = new QGroupBox(groupBox);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(270, 100, 261, 201));
        groupBox_6->setFont(font1);
        groupBox_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(170, 255, 255);"));
        textBrowser_2 = new QTextBrowser(groupBox_6);
        textBrowser_2->setObjectName("textBrowser_2");
        textBrowser_2->setGeometry(QRect(10, 20, 241, 171));
        textBrowser_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(10, 100, 261, 201));
        groupBox_5->setFont(font1);
        groupBox_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(170, 255, 255);"));
        textBrowser = new QTextBrowser(groupBox_5);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(10, 20, 241, 171));
        textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(560, 20, 631, 531));
        tab = new QWidget();
        tab->setObjectName("tab");
        customPlotWidget = new QWidget(tab);
        customPlotWidget->setObjectName("customPlotWidget");
        customPlotWidget->setGeometry(QRect(10, 10, 601, 481));
        customPlotWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1205, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "DIEU KHIEN", nullptr));
        pushButton_RUN->setText(QCoreApplication::translate("MainWindow", "RUN", nullptr));
        label_DK->setText(QString());
        pushButton_STOP->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "THIET LAP", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "SETPOINT:", nullptr));
        pushButton_STOP_2->setText(QCoreApplication::translate("MainWindow", "OK", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "DU LIEU GUI", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "DU LIEU NHAN ", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
