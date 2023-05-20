#pragma once

#include <QApplication>

#include "Strings.h"
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
            window.setWindowTitle(Strings::WindowTitle);
            window.show();
            app.exec();
            return 0;
        }
    };
}
