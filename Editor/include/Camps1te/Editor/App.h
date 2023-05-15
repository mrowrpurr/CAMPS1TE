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

#define msgbox(...) QMessageBox::information(nullptr, "", string_format(__VA_ARGS__).c_str())

namespace Camps1te::Editor {
    // constexpr auto jsonFilePath = ":/data1.json";
    constexpr auto jsonFilePath = "../../../../Resources/Data/DataFile1.json";

    nlohmann::json _jsonDocument;

    QTableView* colorPaletteTable;
    QTableView* texturesTable;

    class App {
        nlohmann::json LoadJson() {
            if (_jsonDocument.is_null()) {
                QFile jsonFile{jsonFilePath};
                jsonFile.open(QIODevice::ReadOnly);
                auto jsonText = jsonFile.readAll().toStdString();
                jsonFile.close();
                _jsonDocument = nlohmann::json::parse(jsonText);
            }
            return _jsonDocument;
        }

        void SaveJson() {
            QFile jsonFile{jsonFilePath};
            jsonFile.open(QIODevice::WriteOnly);
            jsonFile.write(_jsonDocument.dump(4).c_str());
            jsonFile.close();
        }

        nlohmann::json GetMyModData() { return LoadJson()["data"]["my mod"]; }

        auto GetMapInfo() { return GetMyModData()["maps"]["First Map"]; }

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
                rect->SetImage(QImage(path.c_str()));
                rect->update();
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

        void LoadAvailableTextures(QStandardItemModel& model) {
            model.appendRow(new QStandardItem("<None>"));
            auto textures = GetMyModData()["textures"];
            for (auto& [textureName, textureInfo] : textures.items()) {
                auto textureSource = textureInfo["source"];
                auto textureType   = textureSource["type"].get<std::string>();
                if (textureType == "path") {
                    auto  texturePath = textureSource["data"].get<std::string>();
                    QIcon icon{texturePath.c_str()};
                    auto* name = new QStandardItem(icon, textureName.c_str());
                    auto* path = new QStandardItem(texturePath.c_str());
                    model.appendRow({name, path});
                }
            }
        }

        std::string GetCurrentlySelectedColorNameOrEmpty() {
            if (colorPaletteTable->selectionModel()->selectedRows().empty()) return {};
            auto colorName = colorPaletteTable->selectionModel()
                                 ->selectedRows()
                                 .first()
                                 .data(Qt::DisplayRole)
                                 .toString()
                                 .toStdString();
            if (colorName == "<None>") return {};
            else return colorName;
        }

        std::string GetCurrentlySelectedTextureNameOrEmpty() {
            if (texturesTable->selectionModel()->selectedRows().empty()) return {};
            auto textureName = texturesTable->selectionModel()
                                   ->selectedRows()
                                   .first()
                                   .data(Qt::DisplayRole)
                                   .toString()
                                   .toStdString();
            if (textureName == "<None>") return {};
            else return textureName;
        }

        nlohmann::json GetMapCellInfo(int x, int y) {
            auto tiles = GetMapInfo()["tiles"];
            auto key   = string_format("{},{}", x, y);
            if (tiles.contains(key)) return tiles[key];
        }

        void UpdateMapViewTexture(UI::MapCellGraphicsRectItem* mapCellRect, int x, int y) {
            auto selectedTextureName = GetCurrentlySelectedTextureNameOrEmpty();

            auto textureInfo = GetMyModData()["textures"][selectedTextureName];
            auto textureType = textureInfo["source"]["type"].get<std::string>();
            auto texturePath = textureInfo["source"]["data"].get<std::string>();
            mapCellRect->SetImage(QImage(texturePath.c_str()));

            auto mapCellInfo = GetMapCellInfo(x, y);
            if (mapCellInfo.is_null()) {
                msgbox("No map cell info at {},{}", x, y);
                return;
            }
            if (!mapCellInfo.contains("layers")) mapCellInfo["layers"] = nlohmann::json::array();
            mapCellInfo.clear();
            mapCellInfo["layers"].push_back({
                {"type", "image"                                                },
                {"data", {{"source", {{"type", "path"}, {"data", texturePath}}}}},
            });

            RenderTile(mapCellRect, mapCellInfo);
            _jsonDocument["data"]["my mod"]["maps"]["First Map"]["tiles"]
                         [string_format("{},{}", x, y)] = mapCellInfo;
            SaveJson();
        }

        void UpdateMapViewCellColor(UI::MapCellGraphicsRectItem* mapCellRect, int x, int y) {
            auto selectedColorName = GetCurrentlySelectedColorNameOrEmpty();

            std::vector<int> colors;
            if (selectedColorName == "Red") colors = {255, 0, 0, 255};
            else if (selectedColorName == "Green") colors = {0, 255, 0, 255};
            else if (selectedColorName == "Blue") colors = {0, 0, 255, 255};
            else if (selectedColorName == "Yellow") colors = {255, 255, 0, 255};
            else if (selectedColorName == "Cyan") colors = {0, 255, 255, 255};
            else if (selectedColorName == "Magenta") colors = {255, 0, 255, 255};
            else if (selectedColorName == "Black") colors = {0, 0, 0, 255};
            else if (selectedColorName == "White") colors = {255, 255, 255, 255};
            else if (selectedColorName == "Transparent") colors = {0, 0, 0, 0};
            else {
                msgbox("Unknown color: {}", selectedColorName);
                return;
            }

            auto mapCellInfo = GetMapCellInfo(x, y);
            if (mapCellInfo.is_null()) {
                msgbox("No map cell info at {},{}", x, y);
                return;
            }

            // Totally replace all layers with this color!
            if (!mapCellInfo.contains("layers")) mapCellInfo["layers"] = nlohmann::json::array();
            mapCellInfo.clear();
            mapCellInfo["layers"].push_back({
                {"type", "color"},
                {"data", colors }
            });

            RenderTile(mapCellRect, mapCellInfo);
            _jsonDocument["data"]["my mod"]["maps"]["First Map"]["tiles"]
                         [string_format("{},{}", x, y)] = mapCellInfo;
            SaveJson();
        }

        void OnMapViewCellClick(UI::MapCellGraphicsRectItem* mapCellRect, int x, int y) {
            auto selectedTextureName = GetCurrentlySelectedTextureNameOrEmpty();
            auto selectedColorName   = GetCurrentlySelectedColorNameOrEmpty();
            if (!selectedTextureName.empty()) {
                UpdateMapViewTexture(mapCellRect, x, y);
            } else if (!selectedColorName.empty()) {
                UpdateMapViewCellColor(mapCellRect, x, y);
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

            // Dockable Images for Tiles and Objects
            QStandardItemModel availableTexturesModel(0, 2);
            availableTexturesModel.setHeaderData(0, Qt::Horizontal, "Name");
            availableTexturesModel.setHeaderData(1, Qt::Horizontal, "Source");
            LoadAvailableTextures(availableTexturesModel);
            texturesTable = new QTableView();
            texturesTable->setModel(&availableTexturesModel);
            texturesTable->setSortingEnabled(true);
            texturesTable->sortByColumn(0, Qt::AscendingOrder);
            texturesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
            texturesTable->setSelectionMode(QAbstractItemView::SingleSelection);
            texturesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            texturesTable->horizontalHeader()->setStretchLastSection(true);
            texturesTable->verticalHeader()->hide();
            auto texturesDock = new QDockWidget("Tile Textures", &mainWindow);
            texturesDock->setMinimumWidth(350);
            texturesDock->setWidget(texturesTable);

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
            model.appendRow(new QStandardItem("<None>"));
            model.appendRow(new QStandardItem(QIcon(redPixmap), "Red"));
            model.appendRow(new QStandardItem(QIcon(greenPixmap), "Green"));
            model.appendRow(new QStandardItem(QIcon(bluePixmap), "Blue"));

            // Now a table...
            colorPaletteTable = new QTableView();
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
            auto  cellSize = 50;
            auto  padding  = 0;
            auto* scene    = new UI::MapGraphicsScene();
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    auto* rect = new UI::MapCellGraphicsRectItem(
                        i * cellSize, j * cellSize, cellSize - padding, cellSize - padding
                    );
                    auto key = string_format("{},{}", j, i);
                    if (mapInfo["tiles"].contains(key)) {
                        auto tile = mapInfo["tiles"][key];
                        RenderTile(rect, tile);
                    }
                    rect->OnClick([i, j, this, rect]() { this->OnMapViewCellClick(rect, j, i); });
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
            mainWindow.addDockWidget(Qt::LeftDockWidgetArea, texturesDock);
            mainWindow.show();

            return app.exec();
        }
    };
}
