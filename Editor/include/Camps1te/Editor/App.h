#pragma once

#include <_Log_.h>
#include <string_format.h>

#include <QApplication>
#include <QFile>
#include <memory>
#include <nlohmann/json.hpp>

#include "../UI/MapCellGraphicsRectItem.h"
#include "../UI/MapGraphicsScene.h"
#include "../UI/MapGraphicsView.h"

namespace Camps1te::Editor {

    class App {
        nlohmann::json _json;

        void LoadJson() {
            _Log_("App::LoadJson()");
            QFile jsonFile{":/data1.json"};
            jsonFile.open(QIODevice::ReadOnly);
            auto jsonText = jsonFile.readAll().toStdString();
            jsonFile.close();
            _json = nlohmann::json::parse(jsonText);
            _Log_("App::LoadJson() - done");
        }

    public:
        int Run(int argc, char* argv[]) {
            _Log_("App::Run()");
            LoadJson();

            auto mapInfo = _json["data"]["my mod"]["maps"]["First Map"];
            auto name    = mapInfo.value("name", "");
            auto columns = mapInfo.value("columns", 0);
            auto rows    = mapInfo.value("rows", 0);

            QApplication app(argc, argv);

            auto* scene = new UI::MapGraphicsScene();
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    auto* rect = new UI::MapCellGraphicsRectItem(i * 50, j * 50, 45, 45);
                    auto  key  = string_format("{},{}", j, i);
                    if (mapInfo["tiles"].contains(key)) {
                        auto tile = mapInfo["tiles"][key];
                        if (tile.contains("color")) {
                            auto color = tile["color"].get<std::vector<int>>();
                            auto red   = color[0];
                            auto green = color[1];
                            auto blue  = color[2];
                            auto alpha = color[3];
                            rect->setBrush(QBrush(QColor(red, green, blue, alpha)));
                        }
                    }
                    scene->addItem(rect);
                }
            }

            auto* view = new UI::MapGraphicsView();
            view->setScene(scene);
            view->setWindowTitle(name.c_str());
            view->show();

            return app.exec();
        }
    };
}
