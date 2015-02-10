#include <QApplication>
#include <mainwindow.h>
#include <stackimage.h>
#include <drawzone.h>

int main(int argc, char** argv){

    QApplication app(argc, argv);

    MainWindow* mainWindow = new MainWindow();
    //mainWindow->show();

    DrawZone* drawZone = new DrawZone(300,300);
    //drawZone->show();

    StackImage* stack = new StackImage();
    stack->push(drawZone);
    stack->push("fedora.png",150);
    stack->show();

    return app.exec();
}
