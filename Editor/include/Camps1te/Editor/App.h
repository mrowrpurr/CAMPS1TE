#pragma once

#include <Camps1te/Data.h>
#include <_Log_.h>

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

            Data::JsonDataFile dataFile1(DataFile1Path);

            // Get the different types of data from the data file.
            std::vector<std::string> types;

            for (auto& entry : dataFile1.GetEntries()) {
                _Log_("Entry: {} {}", entry->GetIdentifier(), entry->GetType());
                types.push_back(entry->GetType());
            }

            // For each type, we'll add children for each identifier

            window.show();
            return app.exec();
        }
    };
}
