#pragma once

#include <QApplication>
#include <QIcon>

#include "Strings.h"
#include "Windows/Q_DataFilesSelector.h"

namespace Camps1te::Editor {

    class App {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app{argc, argv};

    public:
        int Run(int argc, char* argv[]) {
            QIcon item{":icon.png"};
            app.setWindowIcon(item);
#ifdef _WIN32
            app.setStyle("fusion");
#endif
            app.setStyleSheet("QLineEdit[invalid=true] { color: red; }");
            DataFilesSelectorWindow window;
            window.setWindowTitle(Strings::WindowTitle);
            window.setMinimumWidth(600);
            window.show();
            app.exec();
            return 0;
        }
    };
}
