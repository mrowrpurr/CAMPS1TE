#pragma once

#include <QApplication>
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>


namespace Camps1te::Editor::MiscExamples::ImageWindow {

    void Run() {
        int          argc   = 0;
        char*        argv[] = {nullptr};
        QApplication app(argc, argv);

        // Create the main window
        QWidget mainWindow;
        mainWindow.setWindowTitle("Image Viewer");

        // Create the layout
        QVBoxLayout layout(&mainWindow);

        // Create the image label
        QLabel  imageLabel;
        QPixmap image(":/logo.png");
        imageLabel.setPixmap(image);
        imageLabel.setAlignment(Qt::AlignHCenter);

        // Add the image label to the layout
        layout.addWidget(&imageLabel);

        QLabel label("Camps1te Editor");
        QFont  font("Serif", 24, QFont::Bold);
        label.setFont(font);
        label.setAlignment(Qt::AlignHCenter);
        layout.addWidget(&label);

        // Create the buttons
        QPushButton button1("Button 1");
        QPushButton button2("Button 2");

        // Add the buttons to the layout
        layout.addWidget(&button1);
        layout.addWidget(&button2);

        // Show the main window
        mainWindow.show();

        // Run the application
        app.exec();
    }
}
