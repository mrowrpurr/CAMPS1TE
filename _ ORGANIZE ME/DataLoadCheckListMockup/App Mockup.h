#pragma once

#include <Camps1te/Data.h>

#include <QApplication>
#include <QDragMoveEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardItem>
#include <QTreeView>
#include <QVBoxLayout>

#include "MocMe.h"

class CustomTreeView : public QTreeView {
public:
    CustomTreeView(QWidget* parent = nullptr) : QTreeView(parent) {}

protected:
    void dragMoveEvent(QDragMoveEvent* event) override {
        // Always set the drop action to move (not copy or link)
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
};

namespace Camps1te::Editor {

    constexpr auto DataFile1Path = "../../../../Resources/Data/DataFile1.json";
    constexpr auto DataFile2Path = "../../../../Resources/Data/DataFile2.json";
    constexpr auto QssFilePath   = "../../../../Resources/Editor/Editor.qss";

    class App {
    public:
        int Run(int argc, char* argv[]) {
            QApplication app(argc, argv);
            QMainWindow mainWindow;
            QWidget     window;

            QVBoxLayout layout(&window);

            // Create and set model
            CustomStandardItemModel model;
            model.setColumnCount(3);
            model.setHeaderData(0, Qt::Horizontal, "Select");
            model.setHeaderData(1, Qt::Horizontal, "Description");
            model.setHeaderData(2, Qt::Horizontal, "Order");

            // Add items to the model
            QList<QPair<QString, QString>> items{
                {"First",  "This is the first item" },
                {"Second", "This is the second item"},
                {"Third",  "This is the third item" }
            };

            for (int i = 0; i < items.size(); ++i) {
                const auto&    pair  = items[i];
                QStandardItem* item1 = new QStandardItem(pair.first);
                item1->setCheckable(true);
                QStandardItem* item2 = new QStandardItem(pair.second);
                QStandardItem* item3 = new QStandardItem();
                item3->setData(i, Qt::DisplayRole);
                model.appendRow({item1, item2, item3});
            }
            model.updateOrder();

            // Create and add tree view
            QTreeView treeView;
            treeView.setModel(&model);
            treeView.setSortingEnabled(true);
            treeView.setDragDropMode(QAbstractItemView::InternalMove);
            treeView.setDragEnabled(true);
            treeView.setAcceptDrops(true);
            treeView.setDropIndicatorShown(true);
            layout.addWidget(&treeView);

            // Create and add button
            QPushButton button("Show Message");
            layout.addWidget(&button);

            QObject::connect(&button, &QPushButton::clicked, [&]() {
                QString message = "Checkbox selections:\n";
                for (int i = 0; i < model.rowCount(); ++i) {
                    QStandardItem* item1 = model.item(i, 0);
                    QStandardItem* item2 = model.item(i, 1);
                    if (item1->checkState() == Qt::Checked)
                        message += item1->text() + ": " + item2->text() + "\n";
                }

                QMessageBox::information(&window, "Selections", message);
            });

            mainWindow.setCentralWidget(&window);
            mainWindow.show();
            return app.exec();
        }
    };
}
