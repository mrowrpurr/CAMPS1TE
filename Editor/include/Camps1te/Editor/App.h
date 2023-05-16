#pragma once

#include <Camps1te/Data.h>

#include <QApplication>
#include <QMainWindow>

namespace Camps1te::Editor {

    constexpr auto DataFile1Path = "../../../../Resources/Data/DataFile1.json";
    constexpr auto DataFile2Path = "../../../../Resources/Data/DataFile2.json";
    constexpr auto QssFilePath   = "../../../../Resources/Editor/Editor.qss";

    class App {
    public:
        int Run(int argc, char* argv[]) {
            Data::DataFile* dataFile1 = new Data::JsonDataFile{DataFile1Path};

            auto pluginNames = dataFile1->GetPluginNames();
            _Log_("Loaded {} plugin names from {}", pluginNames.size(), DataFile1Path);
            for (const auto& pluginName : pluginNames) {
                _Log_("Plugin name: {}", pluginName);
                auto dataTypes = dataFile1->GetDataTypes(pluginName);
                _Log_("Loaded {} data types from {}", dataTypes.size(), pluginName);
                for (const auto& dataType : dataTypes) {
                    _Log_("Data type: {}", dataType);
                }
            }

            QApplication app(argc, argv);
            QMainWindow  window;
            window.show();
            return app.exec();
        }
    };
}
