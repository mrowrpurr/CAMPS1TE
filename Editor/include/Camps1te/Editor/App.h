#pragma once

#include <QApplication>
#include <memory>

#include "MiscExamples/ImagesInLists.h"

namespace Camps1te::Editor {

    class App {
    public:
        int Run(int argc, char* argv[]) {
            // QApplication app(argc, argv);

            // Do things!
            MiscExamples::ImagesInLists::Run(argc, argv);

            // return app.exec();
            return true;
        }
    };
}
