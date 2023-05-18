#pragma once

// #include <Camps1te/Data/Json.h>
#include <_Log_.h>

#include <QApplication>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTreeView>
#include <QWidget>

namespace Camps1te::Editor {

    constexpr auto DataFile1Path = "../../../../Resources/Data/DataFile1.json";
    constexpr auto DataFile2Path = "../../../../Resources/Data/DataFile2.json";
    constexpr auto QssFilePath   = "../../../../Resources/Editor/Editor.qss";

    class App {
    public:
        int Run(int argc, char* argv[]) {
            // QApplication app(argc, argv);
            // app.setStyleSheet(
            //     "QWidget, QHeaderView::section { font-size: 20px; color: white; background-color:
            //     " "black; }"
            // );
            // QMainWindow window;

            // Data::JsonDataFile dataFile1(DataFile1Path);
            // auto               entries = dataFile1.GetEntries();

            // //

            // Data::JsonDataStore dataStore;

            // // Create and set up model
            // QStandardItemModel* model = new QStandardItemModel(&window);
            // model->setHorizontalHeaderLabels({"Type", "Description"});

            // // Create tree structure
            // std::map<std::string, QStandardItem*> parents;
            // for (const auto& entry : entries) {
            //     auto type        = entry->GetType();
            //     auto identifier  = entry->GetIdentifier();
            //     auto description = entry->GetString("description");

            //     QStandardItem* parentItem = nullptr;
            //     auto           it         = parents.find(type);
            //     if (it == parents.end()) {
            //         parentItem = new QStandardItem(QString::fromStdString(type));
            //         model->appendRow(parentItem);
            //         parents[type] = parentItem;
            //     } else {
            //         parentItem = it->second;
            //     }

            //     QList<QStandardItem*> rowItems;
            //     rowItems.append(new QStandardItem(QString::fromStdString(identifier)));
            //     rowItems.append(new QStandardItem(QString::fromStdString(description)));
            //     parentItem->appendRow(rowItems);
            // }

            // // Create and set up view
            // QTreeView* view = new QTreeView(&window);
            // view->setModel(model);

            // // Set the tree view as the central widget
            // window.setCentralWidget(view);

            // window.show();
            // return app.exec();
            return true;
        }
    };
}
