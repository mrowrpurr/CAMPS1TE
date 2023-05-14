#pragma once

#include <QAction>
#include <QApplication>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>

namespace Camps1te::Editor::MiscExamples::MenuStatusToolbar {

    int Run(int argc, char* argv[]) {
        QApplication app(argc, argv);

        QMainWindow mainWindow;

        // Menu bar
        QMenuBar* menuBar         = mainWindow.menuBar();
        QMenu*    fileMenu        = menuBar->addMenu("File");
        QMenu*    preferencesMenu = fileMenu->addMenu("Preferences");
        QAction*  somethingAction =
            preferencesMenu->addAction(QIcon(":/resources/something.png"), "Something");
        somethingAction->setShortcut(QKeySequence("Ctrl+S"));
        QMenu*   helpMenu    = menuBar->addMenu("Help");
        QAction* aboutAction = helpMenu->addAction("About");
        aboutAction->setShortcut(QKeySequence("F1"));

        QObject::connect(aboutAction, &QAction::triggered, [&]() {
            QMessageBox::information(&mainWindow, "About", "This is a Qt Application.");
        });

        // Status bar
        mainWindow.statusBar()->showMessage("Ready");

        // Tool bar
        QToolBar* toolBar = mainWindow.addToolBar("Tools");
        toolBar->addAction(somethingAction);

        mainWindow.show();
        return app.exec();
    }
}
