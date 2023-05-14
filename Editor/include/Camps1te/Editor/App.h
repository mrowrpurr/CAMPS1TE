#pragma once

#include <QApplication>
#include <memory>

#include "MiscExamples/MapView.h"

namespace Camps1te::Editor {

    class App {
    public:
        int Run(int argc, char* argv[]) {
            // QApplication app(argc, argv);

            // Do things!
            MiscExamples::MapView::Run(argc, argv);

            // return app.exec();
            return true;
        }
    };
}
