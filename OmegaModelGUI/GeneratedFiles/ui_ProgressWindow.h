/********************************************************************************
** Form generated from reading UI file 'ProgressWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSWINDOW_H
#define UI_PROGRESSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QProgressBar *progressBar;
    QLabel *label;
    QFrame *line;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 119);
        progressBar = new QProgressBar(Form);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(30, 20, 351, 21));
        progressBar->setValue(24);
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 50, 141, 16));
        line = new QFrame(Form);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 70, 381, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(200, 80, 111, 31));
        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(70, 80, 111, 31));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        label->setText(QApplication::translate("Form", "Importing fie......", nullptr));
        pushButton->setText(QApplication::translate("Form", "Quit", nullptr));
        pushButton_2->setText(QApplication::translate("Form", "Done", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSWINDOW_H
