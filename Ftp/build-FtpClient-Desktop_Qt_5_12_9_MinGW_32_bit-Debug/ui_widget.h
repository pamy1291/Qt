/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QListWidget *listWidget_server;
    QListWidget *listWidget_client;
    QListWidget *listWidget_tips;
    QPushButton *pushButton_put;
    QPushButton *pushButton_get;
    QWidget *widget_link;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_user;
    QLineEdit *lineEdit_user;
    QLabel *label_user_pass;
    QLineEdit *lineEdit_pass;
    QLabel *label_ip;
    QLineEdit *lineEdit_ip;
    QLabel *label_port;
    QLineEdit *lineEdit_port;
    QPushButton *pushButton_link;
    QLineEdit *lineEdit_cli_path;
    QLineEdit *lineEdit_srv_path;
    QPushButton *pushButton_entry_srv;
    QPushButton *pushButton_entry_cli;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1186, 738);
        listWidget_server = new QListWidget(Widget);
        listWidget_server->setObjectName(QString::fromUtf8("listWidget_server"));
        listWidget_server->setGeometry(QRect(629, 139, 511, 381));
        listWidget_client = new QListWidget(Widget);
        listWidget_client->setObjectName(QString::fromUtf8("listWidget_client"));
        listWidget_client->setGeometry(QRect(20, 139, 511, 381));
        listWidget_tips = new QListWidget(Widget);
        listWidget_tips->setObjectName(QString::fromUtf8("listWidget_tips"));
        listWidget_tips->setGeometry(QRect(20, 530, 1121, 192));
        pushButton_put = new QPushButton(Widget);
        pushButton_put->setObjectName(QString::fromUtf8("pushButton_put"));
        pushButton_put->setGeometry(QRect(540, 270, 81, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        pushButton_put->setFont(font);
        pushButton_get = new QPushButton(Widget);
        pushButton_get->setObjectName(QString::fromUtf8("pushButton_get"));
        pushButton_get->setGeometry(QRect(540, 350, 81, 41));
        pushButton_get->setFont(font);
        widget_link = new QWidget(Widget);
        widget_link->setObjectName(QString::fromUtf8("widget_link"));
        widget_link->setGeometry(QRect(20, 20, 1121, 61));
        horizontalLayout_2 = new QHBoxLayout(widget_link);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_user = new QLabel(widget_link);
        label_user->setObjectName(QString::fromUtf8("label_user"));

        horizontalLayout_2->addWidget(label_user);

        lineEdit_user = new QLineEdit(widget_link);
        lineEdit_user->setObjectName(QString::fromUtf8("lineEdit_user"));

        horizontalLayout_2->addWidget(lineEdit_user);

        label_user_pass = new QLabel(widget_link);
        label_user_pass->setObjectName(QString::fromUtf8("label_user_pass"));

        horizontalLayout_2->addWidget(label_user_pass);

        lineEdit_pass = new QLineEdit(widget_link);
        lineEdit_pass->setObjectName(QString::fromUtf8("lineEdit_pass"));
        lineEdit_pass->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(lineEdit_pass);

        label_ip = new QLabel(widget_link);
        label_ip->setObjectName(QString::fromUtf8("label_ip"));

        horizontalLayout_2->addWidget(label_ip);

        lineEdit_ip = new QLineEdit(widget_link);
        lineEdit_ip->setObjectName(QString::fromUtf8("lineEdit_ip"));

        horizontalLayout_2->addWidget(lineEdit_ip);

        label_port = new QLabel(widget_link);
        label_port->setObjectName(QString::fromUtf8("label_port"));

        horizontalLayout_2->addWidget(label_port);

        lineEdit_port = new QLineEdit(widget_link);
        lineEdit_port->setObjectName(QString::fromUtf8("lineEdit_port"));

        horizontalLayout_2->addWidget(lineEdit_port);

        pushButton_link = new QPushButton(widget_link);
        pushButton_link->setObjectName(QString::fromUtf8("pushButton_link"));

        horizontalLayout_2->addWidget(pushButton_link);

        horizontalLayout_2->setStretch(1, 2);
        horizontalLayout_2->setStretch(3, 2);
        horizontalLayout_2->setStretch(5, 2);
        horizontalLayout_2->setStretch(7, 1);
        lineEdit_cli_path = new QLineEdit(Widget);
        lineEdit_cli_path->setObjectName(QString::fromUtf8("lineEdit_cli_path"));
        lineEdit_cli_path->setGeometry(QRect(20, 100, 431, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        lineEdit_cli_path->setFont(font1);
        lineEdit_srv_path = new QLineEdit(Widget);
        lineEdit_srv_path->setObjectName(QString::fromUtf8("lineEdit_srv_path"));
        lineEdit_srv_path->setGeometry(QRect(630, 100, 431, 31));
        lineEdit_srv_path->setFont(font1);
        pushButton_entry_srv = new QPushButton(Widget);
        pushButton_entry_srv->setObjectName(QString::fromUtf8("pushButton_entry_srv"));
        pushButton_entry_srv->setGeometry(QRect(1070, 100, 71, 31));
        pushButton_entry_srv->setFont(font1);
        pushButton_entry_cli = new QPushButton(Widget);
        pushButton_entry_cli->setObjectName(QString::fromUtf8("pushButton_entry_cli"));
        pushButton_entry_cli->setGeometry(QRect(461, 101, 71, 31));
        pushButton_entry_cli->setFont(font1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        pushButton_put->setText(QApplication::translate("Widget", "\344\270\212\344\274\240->", nullptr));
        pushButton_get->setText(QApplication::translate("Widget", "<-\344\270\213\350\275\275", nullptr));
        label_user->setText(QApplication::translate("Widget", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_user_pass->setText(QApplication::translate("Widget", "\345\257\206\347\240\201\357\274\232", nullptr));
        label_ip->setText(QApplication::translate("Widget", "ip\345\234\260\345\235\200\357\274\232", nullptr));
        label_port->setText(QApplication::translate("Widget", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        pushButton_link->setText(QApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));
        lineEdit_cli_path->setText(QApplication::translate("Widget", "ClientPath", nullptr));
        lineEdit_srv_path->setText(QApplication::translate("Widget", "ServerPath", nullptr));
        pushButton_entry_srv->setText(QApplication::translate("Widget", "Entry", nullptr));
        pushButton_entry_cli->setText(QApplication::translate("Widget", "Entry", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
