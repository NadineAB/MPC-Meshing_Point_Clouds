/********************************************************************************
** Form generated from reading UI file 'facemodeling.ui'
**
** Created: Fri 6. Sep 00:13:12 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACEMODELING_H
#define UI_FACEMODELING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glCanvas.h"

QT_BEGIN_NAMESPACE

class Ui_FACEMODELINGClass
{
public:
    QAction *actionImport_Scan_Mesh;
    QAction *actionQuit;
    QAction *actionClose_Scan_Mesh;
    QAction *actionRemeshing_using_Spherical_Parmterization;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    GLCanvas *MainOGFrame;
    QGridLayout *gridLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuFilter;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FACEMODELINGClass)
    {
        if (FACEMODELINGClass->objectName().isEmpty())
            FACEMODELINGClass->setObjectName(QString::fromUtf8("FACEMODELINGClass"));
        FACEMODELINGClass->resize(1024, 640);
        actionImport_Scan_Mesh = new QAction(FACEMODELINGClass);
        actionImport_Scan_Mesh->setObjectName(QString::fromUtf8("actionImport_Scan_Mesh"));
        actionQuit = new QAction(FACEMODELINGClass);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionClose_Scan_Mesh = new QAction(FACEMODELINGClass);
        actionClose_Scan_Mesh->setObjectName(QString::fromUtf8("actionClose_Scan_Mesh"));
        actionRemeshing_using_Spherical_Parmterization = new QAction(FACEMODELINGClass);
        actionRemeshing_using_Spherical_Parmterization->setObjectName(QString::fromUtf8("actionRemeshing_using_Spherical_Parmterization"));
        centralWidget = new QWidget(FACEMODELINGClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        MainOGFrame = new GLCanvas(centralWidget);
        MainOGFrame->setObjectName(QString::fromUtf8("MainOGFrame"));
        gridLayout = new QGridLayout(MainOGFrame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        verticalLayout_2->addWidget(MainOGFrame);

        FACEMODELINGClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FACEMODELINGClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFilter = new QMenu(menuBar);
        menuFilter->setObjectName(QString::fromUtf8("menuFilter"));
        FACEMODELINGClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FACEMODELINGClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FACEMODELINGClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FACEMODELINGClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FACEMODELINGClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuFilter->menuAction());
        menuFile->addAction(actionImport_Scan_Mesh);
        menuFile->addAction(actionQuit);
        menuFilter->addAction(actionClose_Scan_Mesh);
        menuFilter->addAction(actionRemeshing_using_Spherical_Parmterization);

        retranslateUi(FACEMODELINGClass);
        QObject::connect(actionQuit, SIGNAL(activated()), FACEMODELINGClass, SLOT(close()));
        QObject::connect(actionImport_Scan_Mesh, SIGNAL(activated()), FACEMODELINGClass, SLOT(showFileOpenDialog()));

        QMetaObject::connectSlotsByName(FACEMODELINGClass);
    } // setupUi

    void retranslateUi(QMainWindow *FACEMODELINGClass)
    {
        FACEMODELINGClass->setWindowTitle(QApplication::translate("FACEMODELINGClass", "FACEMODELING", 0, QApplication::UnicodeUTF8));
        actionImport_Scan_Mesh->setText(QApplication::translate("FACEMODELINGClass", "Import Scan Mesh", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("FACEMODELINGClass", "Quit", 0, QApplication::UnicodeUTF8));
        actionClose_Scan_Mesh->setText(QApplication::translate("FACEMODELINGClass", "Close Scan Mesh", 0, QApplication::UnicodeUTF8));
        actionRemeshing_using_Spherical_Parmterization->setText(QApplication::translate("FACEMODELINGClass", "Remeshing using Spherical Parmterization", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("FACEMODELINGClass", "File", 0, QApplication::UnicodeUTF8));
        menuFilter->setTitle(QApplication::translate("FACEMODELINGClass", "Filter", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FACEMODELINGClass: public Ui_FACEMODELINGClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACEMODELING_H
