/********************************************************************************
** Form generated from reading UI file 'SciVis.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCIVIS_H
#define UI_SCIVIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_SciVisClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    GLWidget *GLWindow;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_open_file;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pushButton_display_solid;
    QPushButton *pushButton_display_wireframes;
    QPushButton *pushButton_display_vertex;
    QGroupBox *groupBox_4;
    QFormLayout *formLayout;
    QPushButton *pushButton_code1;
    QPushButton *pushButton_code2;
    QPushButton *pushButton_code3;
    QPushButton *pushButton_code4;
    QPushButton *pushButton_code5;
    QPushButton *pushButton_code6;
    QPushButton *pushButton_code7;
    QPushButton *pushButton_code8;
    QTextEdit *textEdit_output;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SciVisClass)
    {
        if (SciVisClass->objectName().isEmpty())
            SciVisClass->setObjectName("SciVisClass");
        SciVisClass->resize(1080, 755);
        centralWidget = new QWidget(SciVisClass);
        centralWidget->setObjectName("centralWidget");
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        GLWindow = new GLWidget(centralWidget);
        GLWindow->setObjectName("GLWindow");

        horizontalLayout->addWidget(GLWindow);


        horizontalLayout_2->addLayout(horizontalLayout);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName("groupBox");
        groupBox->setMaximumSize(QSize(200, 16777215));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName("verticalLayout");
        pushButton_open_file = new QPushButton(groupBox);
        pushButton_open_file->setObjectName("pushButton_open_file");

        verticalLayout->addWidget(pushButton_open_file);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName("verticalLayout_4");
        pushButton_display_solid = new QPushButton(groupBox_3);
        pushButton_display_solid->setObjectName("pushButton_display_solid");

        verticalLayout_4->addWidget(pushButton_display_solid);

        pushButton_display_wireframes = new QPushButton(groupBox_3);
        pushButton_display_wireframes->setObjectName("pushButton_display_wireframes");

        verticalLayout_4->addWidget(pushButton_display_wireframes);

        pushButton_display_vertex = new QPushButton(groupBox_3);
        pushButton_display_vertex->setObjectName("pushButton_display_vertex");

        verticalLayout_4->addWidget(pushButton_display_vertex);


        verticalLayout->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName("groupBox_4");
        formLayout = new QFormLayout(groupBox_4);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName("formLayout");
        pushButton_code1 = new QPushButton(groupBox_4);
        pushButton_code1->setObjectName("pushButton_code1");

        formLayout->setWidget(0, QFormLayout::FieldRole, pushButton_code1);

        pushButton_code2 = new QPushButton(groupBox_4);
        pushButton_code2->setObjectName("pushButton_code2");

        formLayout->setWidget(1, QFormLayout::FieldRole, pushButton_code2);

        pushButton_code3 = new QPushButton(groupBox_4);
        pushButton_code3->setObjectName("pushButton_code3");

        formLayout->setWidget(2, QFormLayout::FieldRole, pushButton_code3);

        pushButton_code4 = new QPushButton(groupBox_4);
        pushButton_code4->setObjectName("pushButton_code4");

        formLayout->setWidget(3, QFormLayout::FieldRole, pushButton_code4);

        pushButton_code5 = new QPushButton(groupBox_4);
        pushButton_code5->setObjectName("pushButton_code5");

        formLayout->setWidget(4, QFormLayout::FieldRole, pushButton_code5);

        pushButton_code6 = new QPushButton(groupBox_4);
        pushButton_code6->setObjectName("pushButton_code6");

        formLayout->setWidget(5, QFormLayout::FieldRole, pushButton_code6);

        pushButton_code7 = new QPushButton(groupBox_4);
        pushButton_code7->setObjectName("pushButton_code7");

        formLayout->setWidget(6, QFormLayout::FieldRole, pushButton_code7);

        pushButton_code8 = new QPushButton(groupBox_4);
        pushButton_code8->setObjectName("pushButton_code8");

        formLayout->setWidget(7, QFormLayout::FieldRole, pushButton_code8);


        verticalLayout->addWidget(groupBox_4);

        textEdit_output = new QTextEdit(groupBox);
        textEdit_output->setObjectName("textEdit_output");

        verticalLayout->addWidget(textEdit_output);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_2->addWidget(groupBox);

        SciVisClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SciVisClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 906, 22));
        SciVisClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SciVisClass);
        mainToolBar->setObjectName("mainToolBar");
        SciVisClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SciVisClass);
        statusBar->setObjectName("statusBar");
        SciVisClass->setStatusBar(statusBar);

        retranslateUi(SciVisClass);

        QMetaObject::connectSlotsByName(SciVisClass);
    } // setupUi

    void retranslateUi(QMainWindow *SciVisClass)
    {
        SciVisClass->setWindowTitle(QCoreApplication::translate("SciVisClass", "SciVis", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SciVisClass", "Tool", nullptr));
        pushButton_open_file->setText(QCoreApplication::translate("SciVisClass", "Open File", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("SciVisClass", "Display Mode", nullptr));
        pushButton_display_solid->setText(QCoreApplication::translate("SciVisClass", "Solid", nullptr));
        pushButton_display_wireframes->setText(QCoreApplication::translate("SciVisClass", "Wireframes", nullptr));
        pushButton_display_vertex->setText(QCoreApplication::translate("SciVisClass", "Vertex", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("SciVisClass", "Run", nullptr));
        pushButton_code1->setText(QCoreApplication::translate("SciVisClass", "Contours", nullptr));
        pushButton_code2->setText(QCoreApplication::translate("SciVisClass", "Height", nullptr));
        pushButton_code3->setText(QCoreApplication::translate("SciVisClass", "Rainbow Map", nullptr));
        pushButton_code4->setText(QCoreApplication::translate("SciVisClass", "Bicolor Map", nullptr));
        pushButton_code5->setText(QCoreApplication::translate("SciVisClass", "Greyscale Map", nullptr));
        pushButton_code6->setText(QCoreApplication::translate("SciVisClass", "Code 6", nullptr));
        pushButton_code7->setText(QCoreApplication::translate("SciVisClass", "Code 7", nullptr));
        pushButton_code8->setText(QCoreApplication::translate("SciVisClass", "Code 8", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SciVisClass: public Ui_SciVisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCIVIS_H
