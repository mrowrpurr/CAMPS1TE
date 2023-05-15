#pragma once

#include <QApplication>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace Camps1te::Editor::MiscExamples::ChooserAndList {

    int Run(int argc, char* argv[]) {
        QApplication app(argc, argv);

        // Create main window and layout
        QWidget     window;
        QVBoxLayout layout(&window);

        // Create and add combo box
        QComboBox comboBox;
        comboBox.addItem("Choose your thing");
        comboBox.addItem("Foo");
        comboBox.addItem("Bar");
        layout.addWidget(&comboBox);

        // Create and add tree widget
        QTreeWidget treeWidget;
        treeWidget.setColumnCount(1);
        treeWidget.setHeaderHidden(true);

        // Add items to the tree widget
        QStringList items{"First", "Second", "Third"};
        for (const auto& item : items) {
            QTreeWidgetItem* treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, item);
            treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
            treeItem->setCheckState(0, Qt::Unchecked);
        }

        layout.addWidget(&treeWidget);

        // Create and add button
        QPushButton button("Show Message");
        layout.addWidget(&button);

        QObject::connect(&button, &QPushButton::clicked, [&]() {
            QString message = "You selected: " + comboBox.currentText() + "\n";

            message += "Checkbox selections:\n";
            for (int i = 0; i < treeWidget.topLevelItemCount(); ++i) {
                QTreeWidgetItem* item = treeWidget.topLevelItem(i);
                if (item->checkState(0) == Qt::Checked) message += item->text(0) + "\n";
            }

            QMessageBox::information(&window, "Selections", message);
        });

        window.show();

        return app.exec();
    }
}