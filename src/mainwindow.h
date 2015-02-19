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
#include "drawzone.h"
#include <QColor>
#include <QInputDialog>

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

    // File
    QAction* newProjectAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exportDrawAction;
    QAction* exportDrawWithMovieAction;
    QAction* closeAction;
    QAction* quitAction;
    // Draw
    QAction* undoAction;
    QAction* redoAction;
    QAction* freeDrawAction;
    QAction* lineDrawAction;
    QAction* eraserAction;
    // Layers
    QAction* displayBackgroundMovieAction;
    QAction* onionPeelingsAction;
    QAction* peelingsNumberAction;
    // Goto
    QAction* backAction;
    QAction* nextAction;
    QAction* beginAction;
    QAction* endAction;
    // Viewing
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

    DrawZone *drawzone;

    QCursor *cursors[4];
    const static int CURSOR_BASE = 0;
    const static int CURSOR_FREE = 1;
    const static int CURSOR_LINE = 2;
    const static int CURSOR_ERASER= 3;
    QCursor * toolCursor;

    bool perspective;// true = no project | false = project open
    QString projectName;
    QString projectFullPath;
    QTemporaryDir* workingDir;
    int currentIndex;
    bool backgroundDisplayed;
    bool onionDisplayed;
    const static int DEFAULT_PEELINGS_COUNT = 3;
    int peelingsCount;

public:
    explicit MainWindow(QWidget *parent = 0);

private:
     void createActions();
     void createMenus();

     void updateThumbnails();
     void changeCurrentImage(int index);
     void saveCurrentDraw();
     void setPerspective(bool noProject);

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

    void undo();
    void redo();
    void freeDraw();
    void lineDraw();
    void eraser();
    void changePenColor(QColor color);
    void changePenWidth(int width);

    void displayBackgroundMovie(bool active);
    void onionPeelings(bool active);
    void peelingsNumber();

    void back();
    void next();
    void begin();
    void end();
    void goFrame();

    void playFromBeginning();
    void playWithMovie();

    void about();

    void mouseEnterDrawZone();
    void mouseLeaveDrawZone();
    void thumbClick(int index);
};

#endif // MAINWINDOW_H
