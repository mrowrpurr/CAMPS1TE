#pragma once

#include <string_format.h>

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

#pragma region Widget Variables
    QVBoxLayout                    _layout_Window;
    QHBoxLayout                    _layout_DataFolder;
    QLineEdit                      _txt_DataFolder;
    QPushButton                    _btn_SelectFolder{"Select Folder"};
    QTreeView                      _tree_DataFiles;
    DataFilesListStandardItemModel _model_DataFiles;
    QHBoxLayout                    _layout_DataFiles_Buttons;
    QPushButton                    _btn_DataFiles_SetActive{"Set Active"};
    QPushButton                    _btn_Continue{"Continue"};
#pragma endregion

public:
    DataFilesSelectorWindow(QWidget* parent = nullptr) : QWidget(parent) {
        Configure();
        Layout();
        Events();
        _txt_DataFolder.setText(GetDefaultFolderChooserPath().c_str());
    }

private:
#pragma region Widget Setup

    void Layout() {
        _layout_Window.setObjectName("DataFilesWindow_Layout");
        _layout_DataFolder.setObjectName("DataFilesWindow_DataFolder_Layout");
        _layout_DataFiles_Buttons.setObjectName("DataFilesWindow_DataFiles_Buttons_Layout");

        _layout_DataFolder.addWidget(new QLabel("Data Folder:"));
        _layout_DataFolder.addWidget(&_txt_DataFolder);
        _layout_DataFolder.setStretchFactor(&_txt_DataFolder, 1);
        _layout_Window.addLayout(&_layout_DataFolder);
        _layout_Window.addLayout(&_layout_DataFolder);
        _layout_Window.addWidget(&_btn_SelectFolder);
        _layout_Window.addWidget(&_tree_DataFiles);
        _layout_DataFiles_Buttons.addWidget(&_btn_DataFiles_SetActive);
        _layout_DataFiles_Buttons.addWidget(&_btn_Continue);
        _layout_Window.addLayout(&_layout_DataFiles_Buttons);
        setLayout(&_layout_Window);
    }

    void Configure() {
        _tree_DataFiles.setModel(&_model_DataFiles);
        _tree_DataFiles.setSortingEnabled(true);
        _tree_DataFiles.setDragDropMode(QAbstractItemView::InternalMove);
        _tree_DataFiles.setDragEnabled(true);
        _tree_DataFiles.setAcceptDrops(true);
        _tree_DataFiles.setDropIndicatorShown(true);
        _tree_DataFiles.header()->setSectionResizeMode(0, QHeaderView::Stretch);
        _tree_DataFiles.header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        _btn_DataFiles_SetActive.setDisabled(true);
        _btn_Continue.setDisabled(true);
    }

    void Events() {
        connect(
            &_btn_SelectFolder, &QPushButton::clicked, this,
            &DataFilesSelectorWindow::on_btn_SelectFolder_clicked
        );
        connect(
            &_btn_DataFiles_SetActive, &QPushButton::clicked, this,
            &DataFilesSelectorWindow::on_btn_DataFiles_SetActive_clicked
        );
        connect(
            &_txt_DataFolder, &QLineEdit::textChanged, this,
            &DataFilesSelectorWindow::on_txt_DataFolder_textChanged
        );
        connect(
            &_tree_DataFiles, &QTreeView::clicked, this,
            &DataFilesSelectorWindow::on_tree_DataFiles_clicked
        );
        connect(
            &_model_DataFiles, &DataFilesListStandardItemModel::itemChanged, this,
            &DataFilesSelectorWindow::on_model_DataFiles_itemChanged
        );
        connect(&_btn_Continue, &QPushButton::clicked, [&]() {
            QString message = "Data Files to Load:\n";
            for (int i = 0; i < _model_DataFiles.rowCount(); ++i) {
                if (_model_DataFiles.item(i, 0)->checkState() == Qt::Checked) {
                    auto name      = _model_DataFiles.item(i, 0)->text();
                    auto loadOrder = _model_DataFiles.item(i, 1)->data(Qt::DisplayRole).toInt();
                    auto isActive  = _model_DataFiles.item(i, 2)->data(Qt::DisplayRole).toBool();
                    if (isActive)
                        message +=
                            string_format("{} [{}] (Active)\n", name.toStdString(), loadOrder)
                                .c_str();
                    else
                        message +=
                            string_format("{} [{}]\n", name.toStdString(), loadOrder).c_str();
                }
            }
            QMessageBox::information(this, "Data Files", message);
        });
    }

#pragma endregion
#pragma region Event Handlers

    void on_btn_SelectFolder_clicked() {
        auto dir = QFileDialog::getExistingDirectory(
            this, tr("Choose Data Files Directory"), GetDefaultFolderChooserPath().c_str(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        if (!dir.isEmpty()) _txt_DataFolder.setText(dir);
    }

    void on_tree_DataFiles_clicked(const QModelIndex& index) {
        _btn_DataFiles_SetActive.setDisabled(false);
    }

    void on_btn_DataFiles_SetActive_clicked() {
        QModelIndexList selectedRows = _tree_DataFiles.selectionModel()->selectedRows();
        if (selectedRows.size() == 1) _model_DataFiles.setActive(selectedRows[0].row());
    }

    void on_txt_DataFolder_textChanged(const QString& newPath) { ChangeFolder(newPath); }

    void on_model_DataFiles_itemChanged(QStandardItem* item) {
        if (_model_DataFiles.IsPerformingReorder()) return;

        if (item->isCheckable() && item->checkState() == Qt::Unchecked)
            _model_DataFiles.setActive(item->row(), false);

        // If at least 1 item is checked then enable btn_Continue
        for (int i = 0; i < _model_DataFiles.rowCount(); ++i) {
            QStandardItem* checkboxItem = _model_DataFiles.item(i, 0);
            if (checkboxItem->checkState() == Qt::Checked) {
                _btn_Continue.setDisabled(false);
                return;
            }
        }
        _btn_Continue.setDisabled(true);
    }

#pragma endregion
#pragma region Private Functions

    std::filesystem::path GetDefaultFolderChooserPath() {
        // Might want the home dir in the future...
        // auto homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

        // But, for now, let's use the development data folder
        return std::filesystem::canonical(Camps1te::Editor::Paths::Development::DataFolder);
    }

    void ChangeFolder(const QString& newPath) {
        _model_DataFiles.removeRows(0, _model_DataFiles.rowCount());
        _btn_DataFiles_SetActive.setDisabled(true);
        _btn_Continue.setDisabled(true);

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
                _model_DataFiles.appendRow({filenameColumn, loadOrderColumn, isActiveColumn});
                ++i;
            }
            _model_DataFiles.updateOrder();

            // valid directory, so remove the invalid style
            _txt_DataFolder.setProperty("invalid", QVariant(false));
            _txt_DataFolder.style()->unpolish(&_txt_DataFolder);
            _txt_DataFolder.style()->polish(&_txt_DataFolder);
        } else {
            // invalid directory, so apply the invalid style
            _txt_DataFolder.setProperty("invalid", QVariant(true));
            _txt_DataFolder.style()->unpolish(&_txt_DataFolder);
            _txt_DataFolder.style()->polish(&_txt_DataFolder);
        }
    }
#pragma endregion
};
