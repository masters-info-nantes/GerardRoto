#include <QApplication>
#include <mainwindow.h>
#include <stackimage.h>
#include <drawzone.h>

int main(int argc, char** argv){

    QApplication app(argc, argv);

    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

    return app.exec();
}
