#pragma once

#include <_Log_.h>
#include <fmt/format.h>
#include <string_format.h>

#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QFile>
#include <QMainWindow>
#include <memory>
#include <nlohmann/json.hpp>

#include "../UI/MapCellGraphicsRectItem.h"
#include "../UI/MapGraphicsScene.h"
#include "../UI/MapGraphicsView.h"

namespace Camps1te::Editor {

    class App {
        nlohmann::json LoadJson() {
            QFile jsonFile{":/data1.json"};
            jsonFile.open(QIODevice::ReadOnly);
            auto jsonText = jsonFile.readAll().toStdString();
            jsonFile.close();
            return nlohmann::json::parse(jsonText);
        }

        auto GetMapInfo() { return LoadJson()["data"]["my mod"]["maps"]["First Map"]; }

    public:
        int Run(int argc, char* argv[]) {
            // DATA
            auto mapInfo = GetMapInfo();
            auto columns = mapInfo.value("columns", 0);
            auto rows    = mapInfo.value("rows", 0);

            // APP AND WINDOW
            QApplication app(argc, argv);
            QMainWindow  mainWindow;
            mainWindow.setWindowTitle(mapInfo.value("name", "").c_str());
            mainWindow.setMinimumSize(1920, 1080);

            // DOCKABLE COLOR PALETTE
            QComboBox colorPaletteOptions;
            colorPaletteOptions.addItem("Choose your color");
            colorPaletteOptions.addItem("Red");
            colorPaletteOptions.addItem("Green");
            colorPaletteOptions.addItem("Blue");
            auto colorPaletteDock = new QDockWidget("Color Palette", &mainWindow);
            colorPaletteDock->setMinimumWidth(350);
            colorPaletteDock->setWidget(&colorPaletteOptions);

            // DOCKABLE MAP VIEW
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
            auto* mapView = new UI::MapGraphicsView();
            mapView->setScene(scene);

            mainWindow.setCentralWidget(mapView);
            mainWindow.addDockWidget(Qt::RightDockWidgetArea, colorPaletteDock);
            mainWindow.show();

            return app.exec();
        }
    };
}
