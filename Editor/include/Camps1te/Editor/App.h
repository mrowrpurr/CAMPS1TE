#pragma once

#include <Camps1te/Data.h>

#include <QApplication>
#include <QWidget>

namespace Camps1te::Editor {

    constexpr auto DataFile1Path = "../../../../Resources/Data/DataFile1.json";
    constexpr auto DataFile2Path = "../../../../Resources/Data/DataFile2.json";
    constexpr auto QssFilePath   = "../../../../Resources/Editor/Editor.qss";

    class App {
    public:
        int Run(int argc, char* argv[]) {
            QApplication app(argc, argv);
            app.setStyleSheet("QWidget { font-size: 20px; }");
            QWidget window;
            window.show();
            return app.exec();
        }
    };
}
