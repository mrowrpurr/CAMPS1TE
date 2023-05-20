#pragma once

#include <QApplication>

#include "Windows/DataFilesSelector.h"

namespace Camps1te::Editor {

    constexpr auto QssFilePath = "../../../../Resources/Editor/Editor.qss";

    class App {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app{argc, argv};

    public:
        int Run(int argc, char* argv[]) {
            app.setStyleSheet("QLineEdit[invalid=true] { color: red; }");
            DataFilesSelectorWindow window;
            window.show();
            app.exec();
            return true;
        }
    };
}
