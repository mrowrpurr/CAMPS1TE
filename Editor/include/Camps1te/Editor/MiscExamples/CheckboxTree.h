#pragma once

#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace Camps1te::Editor::MiscExamples::CheckboxTree {

    int Run(int argc, char* argv[]) {
        QApplication app(argc, argv);

        // Create main window and layout
        QWidget     window;
        QVBoxLayout layout(&window);

        // Create and add tree widget
        QTreeWidget treeWidget;
        treeWidget.setColumnCount(2);
        QStringList headers;
        headers << "Select"
                << "Description";
        treeWidget.setHeaderLabels(headers);

        // Add items to the tree widget
        QList<QPair<QString, QString>> items{
            {"First",  "This is the first item" },
            {"Second", "This is the second item"},
            {"Third",  "This is the third item" }
        };

        for (const auto& pair : items) {
            QTreeWidgetItem* treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, pair.first);
            treeItem->setText(1, pair.second);
            treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
            treeItem->setCheckState(0, Qt::Unchecked);
        }

        treeWidget.setSortingEnabled(true);
        layout.addWidget(&treeWidget);

        // Create and add button
        QPushButton button("Show Message");
        layout.addWidget(&button);

        QObject::connect(&button, &QPushButton::clicked, [&]() {
            QString message = "Checkbox selections:\n";
            for (int i = 0; i < treeWidget.topLevelItemCount(); ++i) {
                QTreeWidgetItem* item = treeWidget.topLevelItem(i);
                if (item->checkState(0) == Qt::Checked)
                    message += item->text(0) + ": " + item->text(1) + "\n";
            }

            QMessageBox::information(&window, "Selections", message);
        });

        window.show();

        return app.exec();
    }
}