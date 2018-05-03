/********************************************************************************
** Form generated from reading UI file 'OmegaModelGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OMEGAMODELGUI_H
#define UI_OMEGAMODELGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OmegaModelGUIClass
{
public:
    QWidget *centralWidget;
    QListWidget *listWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QFrame *line;
    QLabel *label;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OmegaModelGUIClass)
    {
        if (OmegaModelGUIClass->objectName().isEmpty())
            OmegaModelGUIClass->setObjectName(QStringLiteral("OmegaModelGUIClass"));
        OmegaModelGUIClass->resize(615, 535);
        centralWidget = new QWidget(OmegaModelGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 591, 391));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 450, 121, 31));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(480, 450, 121, 31));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(350, 450, 121, 31));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 433, 591, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 411, 61, 16));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(70, 410, 491, 21));
        toolButton = new QToolButton(centralWidget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(570, 410, 31, 21));
        OmegaModelGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(OmegaModelGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 615, 21));
        OmegaModelGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(OmegaModelGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        OmegaModelGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(OmegaModelGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        OmegaModelGUIClass->setStatusBar(statusBar);

        retranslateUi(OmegaModelGUIClass);

        QMetaObject::connectSlotsByName(OmegaModelGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *OmegaModelGUIClass)
    {
        OmegaModelGUIClass->setWindowTitle(QApplication::translate("OmegaModelGUIClass", "OmegaModelGUI", nullptr));
        pushButton->setText(QApplication::translate("OmegaModelGUIClass", "Serialise", nullptr));
        pushButton_2->setText(QApplication::translate("OmegaModelGUIClass", "Quit", nullptr));
        pushButton_3->setText(QApplication::translate("OmegaModelGUIClass", "Delete All", nullptr));
        label->setText(QApplication::translate("OmegaModelGUIClass", "Destination:", nullptr));
        toolButton->setText(QApplication::translate("OmegaModelGUIClass", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OmegaModelGUIClass: public Ui_OmegaModelGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OMEGAMODELGUI_H
