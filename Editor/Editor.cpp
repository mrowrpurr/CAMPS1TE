#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMessageBox::information(nullptr, "Hello, Qt!", "Hello, world!");

    return app.exec();
}
