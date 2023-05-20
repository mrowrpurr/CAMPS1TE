#pragma once

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QStyle>
#include <QTextEdit>
#include <QTreeView>
#include <QVBoxLayout>
#include <QtGlobal>

#include "DataFilesSelector/Q_DataFilesListStandardItemModel.h"

#ifndef Q_MOC_RUN
    #include "../Paths.h"
#endif

class DataFilesSelectorWindow : public QWidget {
    Q_OBJECT

    QVBoxLayout                    layout_Window;
    QHBoxLayout                    layout_DataFolder;
    QLineEdit                      txt_DataFolder;
    QPushButton                    btn_SelectFolder{"Select Folder"};
    QTreeView                      tree_DataFiles;
    DataFilesListStandardItemModel model_DataFiles;
    QHBoxLayout                    layout_DataFiles_Buttons;
    QPushButton                    btn_DataFiles_SetActive{"Set Active"};
    QPushButton                    btn_Continue{"Continue"};

    void Layout() {
        layout_DataFolder.addWidget(new QLabel("Data Folder:"));
        layout_DataFolder.addWidget(&txt_DataFolder);
        layout_DataFolder.setStretchFactor(&txt_DataFolder, 1);
        layout_Window.addLayout(&layout_DataFolder);
        layout_Window.addLayout(&layout_DataFolder);
        layout_Window.addWidget(&btn_SelectFolder);
        layout_Window.addWidget(&tree_DataFiles);
        layout_DataFiles_Buttons.addWidget(&btn_DataFiles_SetActive);
        layout_DataFiles_Buttons.addWidget(&btn_Continue);
        layout_Window.addLayout(&layout_DataFiles_Buttons);
        setLayout(&layout_Window);
    }

    void Configure() {
        tree_DataFiles.setModel(&model_DataFiles);
        tree_DataFiles.setSortingEnabled(true);
        tree_DataFiles.setDragDropMode(QAbstractItemView::InternalMove);
        tree_DataFiles.setDragEnabled(true);
        tree_DataFiles.setAcceptDrops(true);
        tree_DataFiles.setDropIndicatorShown(true);
        tree_DataFiles.header()->setSectionResizeMode(0, QHeaderView::Stretch);
        tree_DataFiles.header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        btn_DataFiles_SetActive.setDisabled(true);
        btn_Continue.setDisabled(true);
    }

    void Events() {
        connect(
            &btn_SelectFolder, &QPushButton::clicked, this,
            &DataFilesSelectorWindow::on_Click_btn_SelectFolder
        );
        connect(
            &btn_DataFiles_SetActive, &QPushButton::clicked, this,
            &DataFilesSelectorWindow::on_Click_btn_DataFiles_SetActive
        );
        connect(
            &txt_DataFolder, &QLineEdit::textChanged, this,
            &DataFilesSelectorWindow::on_TextChange_txt_DataFolder
        );
        connect(
            &tree_DataFiles, &QTreeView::clicked, this,
            &DataFilesSelectorWindow::on_Click_tree_DataFiles
        );
        connect(
            &model_DataFiles, &DataFilesListStandardItemModel::itemChanged, this,
            &DataFilesSelectorWindow::on_model_DataFiles_itemChanged
        );
        connect(&btn_Continue, &QPushButton::clicked, [&]() {
            QString message = "Checkbox selections:\n";
            for (int i = 0; i < model_DataFiles.rowCount(); ++i) {
                QStandardItem* item1 = model_DataFiles.item(i, 0);
                QStandardItem* item2 = model_DataFiles.item(i, 1);
                if (item1->checkState() == Qt::Checked)
                    message += item1->text() + ": " + item2->text() + "\n";
            }
            QMessageBox::information(this, "Checked Items", message);
        });
    }

    std::filesystem::path GetDefaultFolderChooserPath() {
        // Might want the home dir in the future...
        // auto homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

        // But, for now, let's use the development data folder
        return std::filesystem::canonical(Camps1te::Editor::Paths::Development::DataFolder);
    }

    void ChangeFolder(const QString& newPath) {
        model_DataFiles.removeRows(0, model_DataFiles.rowCount());

        QFileInfo checkFile(newPath);
        if (checkFile.isDir() && checkFile.exists()) {
            QDir        dir(newPath);
            QStringList fileList = dir.entryList(QStringList() << "*.json", QDir::Files);

            unsigned int i = 0;
            for (const QString& file : fileList) {
                auto* filenameColumn = new QStandardItem(file);
                filenameColumn->setCheckable(true);
                auto* loadOrderColumn = new QStandardItem();
                loadOrderColumn->setData(i, Qt::DisplayRole);
                auto* isActiveColumn = new QStandardItem();
                isActiveColumn->setData(false, Qt::DisplayRole);
                model_DataFiles.appendRow({filenameColumn, loadOrderColumn, isActiveColumn});
                ++i;
            }
            model_DataFiles.updateOrder();

            // valid directory, so remove the invalid style
            txt_DataFolder.setProperty("invalid", QVariant(false));
            txt_DataFolder.style()->unpolish(&txt_DataFolder);
            txt_DataFolder.style()->polish(&txt_DataFolder);
            btn_Continue.setDisabled(false);
        } else {
            // invalid directory, so apply the invalid style
            txt_DataFolder.setProperty("invalid", QVariant(true));
            txt_DataFolder.style()->unpolish(&txt_DataFolder);
            txt_DataFolder.style()->polish(&txt_DataFolder);
            btn_Continue.setDisabled(true);
        }
    }

public:
    DataFilesSelectorWindow(QWidget* parent = nullptr) : QWidget(parent) {
        Configure();
        Layout();
        Events();
        txt_DataFolder.setText(GetDefaultFolderChooserPath().c_str());
    }

public slots:
    void on_Click_btn_SelectFolder() {
        auto dir = QFileDialog::getExistingDirectory(
            this, tr("Choose Data Files Directory"), GetDefaultFolderChooserPath().c_str(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        if (!dir.isEmpty()) txt_DataFolder.setText(dir);
    }
    void on_Click_tree_DataFiles(const QModelIndex& index) {
        btn_DataFiles_SetActive.setDisabled(false);
    }
    void on_Click_btn_DataFiles_SetActive() {
        QModelIndexList selectedRows = tree_DataFiles.selectionModel()->selectedRows();
        if (selectedRows.size() == 1) model_DataFiles.setActive(selectedRows[0].row());
    }
    void on_TextChange_txt_DataFolder(const QString& newPath) { ChangeFolder(newPath); }
    void on_model_DataFiles_itemChanged(QStandardItem* item) {
        if (item->isCheckable() && item->checkState() == Qt::Unchecked)
            model_DataFiles.setActive(item->row(), false);
    }
};
