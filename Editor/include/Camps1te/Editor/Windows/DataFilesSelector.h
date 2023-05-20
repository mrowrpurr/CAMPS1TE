#pragma once

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>
#include <QTextEdit>
#include <QVBoxLayout>

class DataFilesSelectorWindow : public QWidget {
    Q_OBJECT

    QLineEdit* inputField;
    QTextEdit* multiLineLabel;

public:
    DataFilesSelectorWindow(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        // Create a QLineEdit.
        inputField = new QLineEdit(this);
        layout->addWidget(inputField);

        // Create a QPushButton.
        QPushButton* button = new QPushButton("Select Folder", this);
        layout->addWidget(button);

        // Create a QTextEdit.
        multiLineLabel = new QTextEdit(this);
        multiLineLabel->setReadOnly(true);
        layout->addWidget(multiLineLabel);

        // Connect signals and slots.
        connect(button, &QPushButton::clicked, this, &DataFilesSelectorWindow::selectFolder);
        connect(
            inputField, &QLineEdit::textChanged, this, &DataFilesSelectorWindow::folderPathChanged
        );
    }

public slots:
    void selectFolder() {
        QString dir = QFileDialog::getExistingDirectory(
            this, tr("Open Directory"), "/home",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        if (!dir.isEmpty()) {
            inputField->setText(dir);
        }
    }

    void folderPathChanged(const QString& newPath) {
        QFileInfo checkFile(newPath);
        if (checkFile.isDir() && checkFile.exists()) {
            QDir        dir(newPath);
            QStringList fileList = dir.entryList(QDir::Files);
            multiLineLabel->setText(fileList.join("\n"));

            // valid directory, so remove the invalid style
            inputField->setProperty("invalid", QVariant(false));
            inputField->style()->unpolish(inputField);
            inputField->style()->polish(inputField);
        } else {
            // invalid directory, so apply the invalid style
            inputField->setProperty("invalid", QVariant(true));
            inputField->style()->unpolish(inputField);
            inputField->style()->polish(inputField);
            multiLineLabel->clear();
        }
    }
};
