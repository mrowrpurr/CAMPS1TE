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
#include <QMenu>
#include <QMenuBar>
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
    // constexpr auto jsonFilePath = ":/data1.json";
    constexpr auto jsonFilePath = "../../../../Resources/Data/DataFile1.json";

    class App {
        nlohmann::json LoadJson() {
            QFile jsonFile{jsonFilePath};
            jsonFile.open(QIODevice::ReadOnly);
            auto jsonText = jsonFile.readAll().toStdString();
            jsonFile.close();
            return nlohmann::json::parse(jsonText);
        }

        auto GetMapInfo() { return LoadJson()["data"]["my mod"]["maps"]["First Map"]; }

        void RenderTileColor(UI::MapCellGraphicsRectItem* rect, const nlohmann::json& color) {
            auto red   = color[0];
            auto green = color[1];
            auto blue  = color[2];
            auto alpha = color[3];
            rect->setBrush(QBrush(QColor(red, green, blue, alpha)));
        }

        void RenderTileImage(UI::MapCellGraphicsRectItem* rect, const nlohmann::json& image) {
            auto type = image["source"]["type"].get<std::string>();
            if (type == "path") {
                auto path = image["source"]["data"].get<std::string>();
                // rect->setBrush(QBrush(QImage(path.c_str())));
                rect->SetImage(QImage(path.c_str()));
            }
        }

        void RenderTileLayer(UI::MapCellGraphicsRectItem* rect, const nlohmann::json& layer) {
            auto type = layer["type"].get<std::string>();
            if (type == "color") RenderTileColor(rect, layer["data"]);
            else if (type == "image") RenderTileImage(rect, layer["data"]);
        }

        void RenderTile(UI::MapCellGraphicsRectItem* rect, const nlohmann::json& tile) {
            if (tile.contains("layers")) {
                auto layers = tile["layers"];
                for (auto& layer : layers) RenderTileLayer(rect, layer);
            }
        }

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
            mainWindow.setMinimumSize(1024, 768);

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
            QStandardItemModel model(0, 1);
            model.setHeaderData(0, Qt::Horizontal, "Name");
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
                        RenderTile(rect, tile);
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

            // MenuBar
            QMenuBar* menuBar = mainWindow.menuBar();

            QMenu*   fileMenu   = menuBar->addMenu("File");
            QAction* exitAction = fileMenu->addAction("Exit");
            exitAction->setShortcut(QKeySequence("Ctrl+Q"));
            QObject::connect(exitAction, &QAction::triggered, [&]() { mainWindow.close(); });

            // Windows menu
            QMenu*   windowsMenu     = menuBar->addMenu("Windows");
            QAction* mapEditorAction = windowsMenu->addAction("Color Palette");
            mapEditorAction->setShortcut(QKeySequence("Ctrl+P"));
            QObject::connect(mapEditorAction, &QAction::triggered, [&]() {
                colorPaletteDock->show();
            });

            QMenu*   helpMenu    = menuBar->addMenu("Help");
            QAction* aboutAction = helpMenu->addAction("About");
            aboutAction->setShortcut(QKeySequence("F1"));
            QObject::connect(aboutAction, &QAction::triggered, [&]() {
                QMessageBox::information(&mainWindow, "About", "CAMPS1TE experimental");
            });

            mainWindow.setCentralWidget(mapView);
            mainWindow.addDockWidget(Qt::RightDockWidgetArea, colorPaletteDock);
            mainWindow.show();

            return app.exec();
        }
    };
}
