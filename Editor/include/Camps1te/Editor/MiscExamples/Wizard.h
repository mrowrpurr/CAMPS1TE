#pragma once

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWizard>
#include <QWizardPage>

namespace Camps1te::Editor::MiscExamples::Wizard {

    class FirstPage : public QWizardPage {
    public:
        FirstPage(QWidget* parent = nullptr) : QWizardPage(parent) {
            setTitle("First Page");
            QLabel* label = new QLabel("This is the first page.", this);
            label->setWordWrap(true);
            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget(label);
            setLayout(layout);
        }
    };

    class SecondPage : public QWizardPage {
    public:
        SecondPage(QWidget* parent = nullptr) : QWizardPage(parent) {
            setTitle("Second Page");
            QLabel* label = new QLabel("This is the second page.", this);
            label->setWordWrap(true);
            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget(label);
            setLayout(layout);
        }
    };

    class Instance {
        QWizard wizard;

    public:
        Instance() {
            wizard.addPage(new FirstPage);
            wizard.addPage(new SecondPage);
            wizard.setWindowTitle("Wizard Example");
            wizard.show();
        }
    };
}
