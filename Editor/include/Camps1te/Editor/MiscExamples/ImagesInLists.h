#pragma once

#include <QApplication>
#include <QHeaderView>
#include <QIcon>
#include <QLineEdit>
#include <QListView>
#include <QSize>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QTreeView>
#include <QVBoxLayout>

namespace Camps1te::Editor::MiscExamples::ImagesInLists {

    int Run(int argc, char* argv[]) {
        QApplication app(argc, argv);

        QStandardItemModel model(0, 3);
        model.setHorizontalHeaderLabels(
            QStringList() << "ID"
                          << "Name"
                          << "Image"
        );

        QIcon folderIcon = QApplication::style()->standardIcon(QStyle::SP_DirIcon);

        // Create items
        QList<QStandardItem*> row1;
        row1.append(new QStandardItem("1"));
        row1.append(new QStandardItem("Item 1"));
        row1.append(new QStandardItem(folderIcon, ""));
        model.appendRow(row1);

        QList<QStandardItem*> row2;
        row2.append(new QStandardItem("2"));
        row2.append(new QStandardItem("Item 2"));
        row2.append(new QStandardItem(folderIcon, ""));
        model.appendRow(row2);

        QSortFilterProxyModel proxyModel;
        proxyModel.setSourceModel(&model);
        proxyModel.setFilterKeyColumn(1);  // Filter based on the "Name" column

        QTableView tableView;
        tableView.setModel(&proxyModel);
        tableView.setSortingEnabled(true);
        tableView.horizontalHeader()->setSortIndicatorShown(true);
        tableView.verticalHeader()->hide();
        tableView.horizontalHeader()->setStretchLastSection(true);

        // Adjust the icon size
        tableView.setIconSize(QSize(100, 100));

        // Text input for filtering
        QLineEdit filterInput;
        QObject::connect(&filterInput, &QLineEdit::textChanged, [&](const QString& text) {
            proxyModel.setFilterFixedString(text);
        });

        // Create a layout
        QVBoxLayout layout;
        layout.addWidget(&filterInput);
        layout.addWidget(&tableView);

        // Create a widget to hold the layout
        QWidget window;
        window.setLayout(&layout);
        window.show();

        return app.exec();
    }

    int Run1(int argc, char* argv[]) {
        QApplication app(argc, argv);

        // TREE

        QIcon          folderIcon  = QApplication::style()->standardIcon(QStyle::SP_DirIcon);
        QStandardItem* folderItem  = new QStandardItem(folderIcon, "Folder");
        QStandardItem* folderItem2 = new QStandardItem(folderIcon, "Folder");

        QStandardItemModel model;

        model.appendRow(folderItem);
        model.appendRow(folderItem2);

        // QTreeView treeView;
        // treeView.setModel(&model);
        // treeView.show();

        // LIST

        QListView listView;
        listView.setModel(&model);

        // Adjust the icon size
        listView.setIconSize(QSize(100, 100));

        listView.show();

        return app.exec();
    }

}
