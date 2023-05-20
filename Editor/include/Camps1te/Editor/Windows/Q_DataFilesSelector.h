#pragma once

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
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

    QVBoxLayout                    layout{this};
    QLineEdit                      txt_dataFolder{this};
    QPushButton                    btn_selectFolder{"Select Folder", this};
    QTextEdit                      lbl_filesList{this};
    QTreeView                      tree_dataFiles;
    DataFilesListStandardItemModel model_dataFiles;
    QPushButton                    btn_accept{"Accept", this};

    void Layout() {
        layout.addWidget(&txt_dataFolder);
        layout.addWidget(&btn_selectFolder);
        layout.addWidget(&lbl_filesList);
        layout.addWidget(&tree_dataFiles);
        layout.addWidget(&btn_accept);
    }

    void Configure() {
        lbl_filesList.setReadOnly(true);
        tree_dataFiles.setModel(&model_dataFiles);
        tree_dataFiles.setSortingEnabled(true);
        tree_dataFiles.setDragDropMode(QAbstractItemView::InternalMove);
        tree_dataFiles.setDragEnabled(true);
        tree_dataFiles.setAcceptDrops(true);
        tree_dataFiles.setDropIndicatorShown(true);
        tree_dataFiles.header()->setSectionResizeMode(0, QHeaderView::Stretch);
        tree_dataFiles.header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    }

    void Events() {
        connect(
            &btn_selectFolder, &QPushButton::clicked, this,
            &DataFilesSelectorWindow::on_ClickSelectFolderButton
        );
        connect(
            &txt_dataFolder, &QLineEdit::textChanged, this,
            &DataFilesSelectorWindow::on_DataFolderTextChanged
        );
        connect(&btn_accept, &QPushButton::clicked, [&]() {
            QString message = "Checkbox selections:\n";
            for (int i = 0; i < model_dataFiles.rowCount(); ++i) {
                QStandardItem* item1 = model_dataFiles.item(i, 0);
                QStandardItem* item2 = model_dataFiles.item(i, 1);
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
        model_dataFiles.removeRows(0, model_dataFiles.rowCount());

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
                model_dataFiles.appendRow({filenameColumn, loadOrderColumn});
                ++i;
            }
            model_dataFiles.updateOrder();

            // valid directory, so remove the invalid style
            txt_dataFolder.setProperty("invalid", QVariant(false));
            txt_dataFolder.style()->unpolish(&txt_dataFolder);
            txt_dataFolder.style()->polish(&txt_dataFolder);
        } else {
            // invalid directory, so apply the invalid style
            txt_dataFolder.setProperty("invalid", QVariant(true));
            txt_dataFolder.style()->unpolish(&txt_dataFolder);
            txt_dataFolder.style()->polish(&txt_dataFolder);
        }
    }

public:
    DataFilesSelectorWindow(QWidget* parent = nullptr) : QWidget(parent) {
        Configure();
        Layout();
        Events();
        txt_dataFolder.setText(GetDefaultFolderChooserPath().c_str());
    }

public slots:
    void on_ClickSelectFolderButton() {
        auto dir = QFileDialog::getExistingDirectory(
            this, tr("Choose Data Files Directory"), GetDefaultFolderChooserPath().c_str(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        if (!dir.isEmpty()) txt_dataFolder.setText(dir);
    }

    void on_DataFolderTextChanged(const QString& newPath) { ChangeFolder(newPath); }
};
