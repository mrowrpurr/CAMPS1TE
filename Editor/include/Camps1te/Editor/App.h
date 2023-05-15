#pragma once

#include <_Log_.h>
#include <fmt/format.h>
#include <string_format.h>

#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QFile>
#include <QHeaderView>
#include <QIcon>
#include <QMainWindow>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
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

            // Let's make it a table so we can show images later!
            // First, here are 4 QIcon for Red, Green, and Blue
            QColor  red(255, 0, 0);
            QPixmap redPixmap(128, 128);
            redPixmap.fill(red);

            QColor  green(0, 255, 0);
            QPixmap greenPixmap(128, 128);
            greenPixmap.fill(green);

            QColor  blue(0, 0, 255);
            QPixmap bluePixmap(128, 128);
            bluePixmap.fill(blue);

            // Now make the model
            QStandardItemModel model(0, 2);
            model.setHeaderData(0, Qt::Horizontal, "Name");
            model.setHeaderData(1, Qt::Horizontal, "Preview");
            model.appendRow(new QStandardItem(QIcon(redPixmap), "Red"));
            model.appendRow(new QStandardItem(QIcon(greenPixmap), "Green"));
            model.appendRow(new QStandardItem(QIcon(bluePixmap), "Blue"));

            // Now a table...
            auto* colorPaletteTable = new QTableView();
            colorPaletteTable->setModel(&model);
            colorPaletteTable->setSortingEnabled(true);
            colorPaletteTable->sortByColumn(0, Qt::AscendingOrder);
            colorPaletteTable->setSelectionBehavior(QAbstractItemView::SelectRows);
            colorPaletteTable->setSelectionMode(QAbstractItemView::SingleSelection);
            colorPaletteTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            colorPaletteTable->horizontalHeader()->setStretchLastSection(true);
            colorPaletteTable->verticalHeader()->hide();

            auto colorPaletteDock = new QDockWidget("Color Palette", &mainWindow);
            colorPaletteDock->setMinimumWidth(350);
            colorPaletteDock->setWidget(colorPaletteTable);

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

                    rect->OnClick([i, j, &colorPaletteTable]() {
                        if (colorPaletteTable->selectionModel()->selectedRows().empty()) {
                            QMessageBox::information(
                                nullptr, "Clicked", string_format("No color selected").c_str()
                            );
                            return;
                        }

                        auto colorName = colorPaletteTable->selectionModel()
                                             ->selectedRows()
                                             .first()
                                             .data(0)
                                             .toString()
                                             .toStdString();
                        QMessageBox::information(
                            nullptr, "Clicked",
                            string_format("Clicked on {},{} with color {}", i, j, colorName).c_str()
                        );
                    });
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
