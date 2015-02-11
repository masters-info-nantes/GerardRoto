#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QListWidget>
#include <QStringListModel>
#include <QButtonGroup>
#include <QFileDialog>
#include <colorpicker.h>
#include "stackimage.h"
#include "newprojectdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QMenu* fileMenu;
    QMenu* drawMenu;
    QMenu* layersMenu;
    QMenu* gotoMenu;
    QMenu* viewingMenu;
    QMenu* aboutMenu;

    QAction* newProjectAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exportDrawAction;
    QAction* exportDrawWithMovieAction;
    QAction* closeAction;
    QAction* quitAction;
    QAction* freeDrawAction;
    QAction* lineDrawAction;
    QAction* eraserAction;
    QAction* displayBackgroundMovieAction;
    QAction* onionPeelingsAction;
    QAction* peelingsNumberAction;
    QAction* beginAction;
    QAction* endAction;
    QAction* backAction;
    QAction* nextAction;
    QAction* playFromBeginningAction;
    QAction* playWithMovieAction;
    QAction* aboutAction;

    //QLabel* imageView;
    StackImage* imageView;

    QWidget* controlsBar;
    QPushButton* buttonBegin;
    QPushButton* buttonEnd;
    QPushButton* buttonBack;
    QPushButton* buttonNext;
    QLineEdit* frameNumber;
    QPushButton* buttonGoFrame;
    QSpinBox* numberPreviousFrames;
    QPushButton* buttonPlayDraws;
    QLabel* labelWithMovie;
    QCheckBox* checkPlayWithMovie;
    QPushButton* buttonPlayFull;

    QWidget* drawBar;
    QPushButton* buttonFreeDraw;
    QPushButton* buttonLineDraw;
    QPushButton* buttonEraser;
    QSpinBox* numberBrushSize;
    ColorPicker* pickerBrushColor;

    QListWidget* thumbnailsList;

public:
    explicit MainWindow(QWidget *parent = 0);

private:
     void createActions();
     void createMenus();

signals:

private slots:
    void newProject();
    void open();
    void save();
    void saveAs();
    void exportDraw();
    void exportDrawWithMovie();
    void close();
    void quit();

    void freeDraw();
    void lineDraw();
    void eraser();

    void displayBackgroundMovie();
    void onionPeelings();
    void peelingsNumber();

    void begin();
    void end();
    void back();
    void next();

    void playFromBeginning();
    void playWithMovie();

    void about();
};

#endif // MAINWINDOW_H
