#pragma once

#include <QApplication>
#include <QFile>
#include <QFileSystemWatcher>
#include <QLabel>
#include <QPushButton>
#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>

namespace Camps1te::Editor::MiscExamples::QmlExample {

    // class QmlWidget : public QWidget {};

    QString loadQSS(const QString& path) {
        QFile file(path);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        return stream.readAll();
    }

    void Run(const std::string& qmlFilePath) {
        int          argc   = 0;
        char*        argv[] = {nullptr};
        QApplication app(argc, argv);
        app.setStyleSheet("* { color: red; }");

        QQmlApplicationEngine engine;
        engine.load(QUrl(std::string{"qrc:/" + qmlFilePath}.c_str()));

        app.exec();
    }
}
