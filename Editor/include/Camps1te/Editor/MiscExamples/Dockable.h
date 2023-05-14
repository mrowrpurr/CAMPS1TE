#pragma once

#include <QApplication>
#include <QDockWidget>
#include <QMainWindow>
#include <QTextEdit>


namespace Camps1te::Editor::MiscExamples::Dockable {

    int Run(int argc, char* argv[]) {
        QApplication app(argc, argv);

        QMainWindow mainWindow;

        // Central widget
        QTextEdit* textEdit = new QTextEdit(&mainWindow);
        mainWindow.setCentralWidget(textEdit);

        // Dock widget
        QDockWidget* dockWidget   = new QDockWidget("Dockable", &mainWindow);
        QTextEdit*   dockTextEdit = new QTextEdit(dockWidget);
        dockWidget->setWidget(dockTextEdit);
        mainWindow.addDockWidget(Qt::RightDockWidgetArea, dockWidget);

        mainWindow.show();
        return app.exec();
    }
}
