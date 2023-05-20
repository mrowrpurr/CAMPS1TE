#pragma once

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>
#include <QTextEdit>
#include <QTreeView>
#include <QVBoxLayout>

#include "DataFilesSelector/DataFilesListStandardItemModel.h"

class DataFilesSelectorWindow : public QWidget {
    Q_OBJECT

    QVBoxLayout layout{this};

    QLineEdit   txt_dataFolder{this};
    QPushButton btn_selectFolder{"Select Folder", this};

    // DataFilesListStandardItemModel dataFilesListModel;
    // We won't use this later:
    QTextEdit* multiLineLabel;

public:
    DataFilesSelectorWindow(QWidget* parent = nullptr) : QWidget(parent) {
        layout.addWidget(&txt_dataFolder);
        layout.addWidget(&btn_selectFolder);

        // Create a QTextEdit.
        multiLineLabel = new QTextEdit(this);
        multiLineLabel->setReadOnly(true);
        layout.addWidget(multiLineLabel);

        // // Create and set model
        // DataFilesListStandardItemModel dataFilesListModel;

        // // Add items to the model
        // QList<QPair<QString, QString>> items{
        //     {"First",  "This is the first item" },
        //     {"Second", "This is the second item"},
        //     {"Third",  "This is the third item" }
        // };
        // for (int i = 0; i < items.size(); ++i) {
        //     const auto&    pair  = items[i];
        //     QStandardItem* item1 = new QStandardItem(pair.first);
        //     item1->setCheckable(true);
        //     QStandardItem* item2 = new QStandardItem(pair.second);
        //     QStandardItem* item3 = new QStandardItem();
        //     item3->setData(i, Qt::DisplayRole);
        //     dataFilesListModel.appendRow({item1, item2, item3});
        // }
        // dataFilesListModel.updateOrder();

        // // Treeview
        // QTreeView treeView;
        // treeView.setModel(&dataFilesListModel);
        // treeView.setSortingEnabled(true);
        // treeView.setDragDropMode(QAbstractItemView::InternalMove);
        // treeView.setDragEnabled(true);
        // treeView.setAcceptDrops(true);
        // treeView.setDropIndicatorShown(true);
        // layout.addWidget(&treeView);

        // // Create and add button
        // QPushButton button2("Show Message");
        // layout.addWidget(&button2);

        // Connect signals and slots.
        connect(
            &btn_selectFolder, &QPushButton::clicked, this, &DataFilesSelectorWindow::selectFolder
        );
        connect(
            &txt_dataFolder, &QLineEdit::textChanged, this,
            &DataFilesSelectorWindow::folderPathChanged
        );
        // connect(&button2, &QPushButton::clicked, [&]() {
        //     QString message = "Checkbox selections:\n";
        //     for (int i = 0; i < dataFilesListModel.rowCount(); ++i) {
        //         QStandardItem* item1 = dataFilesListModel.item(i, 0);
        //         QStandardItem* item2 = dataFilesListModel.item(i, 1);
        //         if (item1->checkState() == Qt::Checked)
        //             message += item1->text() + ": " + item2->text() + "\n";
        //     }
        //     QMessageBox::information(this, "Checked Items", message);
        // });
    }

public slots:
    void selectFolder() {
        QString dir = QFileDialog::getExistingDirectory(
            this, tr("Open Directory"), "/home",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        if (!dir.isEmpty()) {
            txt_dataFolder.setText(dir);
        }
    }

    void folderPathChanged(const QString& newPath) {
        QFileInfo checkFile(newPath);
        if (checkFile.isDir() && checkFile.exists()) {
            QDir        dir(newPath);
            QStringList fileList = dir.entryList(QDir::Files);
            multiLineLabel->setText(fileList.join("\n"));

            // valid directory, so remove the invalid style
            txt_dataFolder.setProperty("invalid", QVariant(false));
            txt_dataFolder.style()->unpolish(&txt_dataFolder);
            txt_dataFolder.style()->polish(&txt_dataFolder);
        } else {
            // invalid directory, so apply the invalid style
            txt_dataFolder.setProperty("invalid", QVariant(true));
            txt_dataFolder.style()->unpolish(&txt_dataFolder);
            txt_dataFolder.style()->polish(&txt_dataFolder);
            multiLineLabel->clear();
        }
    }
};
