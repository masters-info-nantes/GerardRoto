#include "mainwindow.h"
#include "drawzone.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);

    // Create widgets
    this->imageView = new StackImage();
    dz = new DrawZone(1000,600);
    this->imageView->push(dz);
    this->imageView->push("../img/fedora2.png");
    this->imageView->push("../img/hd-1.jpeg");

    this->thumbnailsList = new QListWidget();
    this->thumbnailsList->setFixedHeight(130);
    this->thumbnailsList->setFlow(QListView::LeftToRight);

    for(int i = 1; i < 22; i++){
        QLabel* thumbLabel = new QLabel();
        thumbLabel->setAlignment(Qt::AlignCenter);

        QPixmap* thumbFull = new QPixmap(QDir::currentPath() + "/../img/thumbs/hd-"+ QString::number(i) +".jpeg");
        QPixmap thumbScaled(thumbFull->scaledToHeight(100));

        thumbLabel->setPixmap(thumbScaled);
        thumbLabel->setMinimumSize(QSize(thumbScaled.width(), thumbScaled.height()));

        QListWidgetItem* listItem = new QListWidgetItem(this->thumbnailsList);
        listItem->setSizeHint(QSize(thumbLabel->minimumWidth()+ 30, thumbLabel->minimumHeight()));

        this->thumbnailsList->addItem(listItem);
        this->thumbnailsList->setItemWidget(listItem, thumbLabel);
    }

    QGridLayout* drawLayout = new QGridLayout();
    this->buttonFreeDraw = new QPushButton();
    connect(this->buttonFreeDraw, SIGNAL(clicked()), this, SLOT(freeDraw()));
    this->buttonFreeDraw->setIcon(QIcon(QPixmap("../img/icons/pencil.png")));
    this->buttonFreeDraw->setCheckable(true);
    this->buttonFreeDraw->setChecked(true);

    this->buttonLineDraw = new QPushButton();
    connect(this->buttonLineDraw, SIGNAL(clicked()), this, SLOT(lineDraw()));
    this->buttonLineDraw->setIcon(QIcon(QPixmap("../img/icons/line.png")));
    this->buttonLineDraw->setCheckable(true);

    this->buttonEraser = new QPushButton();
    connect(this->buttonEraser, SIGNAL(clicked()), this, SLOT(eraser()));
    this->buttonEraser->setIcon(QIcon(QPixmap("../img/icons/eraser.png")));
    this->buttonEraser->setCheckable(true);

    QButtonGroup* drawButtonGroup = new QButtonGroup();
    drawButtonGroup->addButton(this->buttonFreeDraw);
    drawButtonGroup->addButton(this->buttonLineDraw);
    drawButtonGroup->addButton(this->buttonEraser);
    drawButtonGroup->setExclusive(true);

    this->numberBrushSize = new QSpinBox();
    connect(this->numberBrushSize, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
    this->pickerBrushColor = new ColorPicker();
    connect(this->pickerBrushColor, SIGNAL(colorChanged(QColor)), this, SLOT(changePenColor(QColor)));

    drawLayout->addWidget(this->buttonFreeDraw, 0, 0);
    drawLayout->addWidget(this->buttonLineDraw, 0, 1);
    drawLayout->addWidget(this->buttonEraser, 1, 0);
    drawLayout->addWidget(this->numberBrushSize, 3, 0, 1, 2);
    drawLayout->addWidget(this->pickerBrushColor, 5, 0, 1, 2);

    drawLayout->setRowMinimumHeight(2, 20);
    drawLayout->setRowMinimumHeight(4, 10);

    drawLayout->setRowStretch(0, 1);
    drawLayout->setRowStretch(1, 1);
    drawLayout->setRowStretch(3, 1);
    drawLayout->setRowStretch(5, 1);
    drawLayout->setRowStretch(6, 5);

    this->drawBar = new QWidget();
    this->drawBar->setFixedWidth(150);
    this->drawBar->setLayout(drawLayout);

    this->controlsBar = new QWidget();
    this->controlsBar->setMaximumSize(1000, 50);

    QHBoxLayout* controlsLayout = new QHBoxLayout();
    this->buttonBegin = new QPushButton();
    this->buttonBegin->setIcon(QIcon(QPixmap("../img/icons/begin.png")));

    this->buttonEnd = new QPushButton();
    this->buttonEnd->setIcon(QIcon(QPixmap("../img/icons/end.png")));

    this->buttonBack = new QPushButton();
    this->buttonBack->setIcon(QIcon(QPixmap("../img/icons/backward.png")));

    this->buttonNext = new QPushButton();
    this->buttonNext->setIcon(QIcon(QPixmap("../img/icons/forward.png")));

    this->frameNumber = new QLineEdit("0");
    this->buttonGoFrame = new QPushButton("Go");
    this->numberPreviousFrames = new QSpinBox();

    this->buttonPlayDraws = new QPushButton();
    this->buttonPlayDraws->setIcon(QIcon(QPixmap("../img/icons/play.png")));

    this->labelWithMovie = new QLabel();
    this->labelWithMovie->setPixmap(QPixmap("../img/icons/movie.png").scaledToHeight(20));

    this->checkPlayWithMovie = new QCheckBox();
    this->buttonPlayFull = new QPushButton("Depuis le début");
    this->buttonPlayFull->setIcon(QIcon(QPixmap("../img/icons/play.png")));

    controlsLayout->addWidget(this->buttonBegin);
    controlsLayout->addWidget(this->buttonBack);

    controlsLayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    controlsLayout->addWidget(new QLabel("Image courante: "));
    controlsLayout->addWidget(this->frameNumber);
    controlsLayout->addWidget(this->buttonGoFrame);

    controlsLayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Minimum));
    controlsLayout->addWidget(this->numberPreviousFrames);
    controlsLayout->addWidget(this->buttonPlayDraws);

    controlsLayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Minimum));
    controlsLayout->addWidget(this->labelWithMovie);
    controlsLayout->addWidget(this->checkPlayWithMovie);
    controlsLayout->addWidget(this->buttonPlayFull);

    controlsLayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Minimum));
    controlsLayout->addWidget(this->buttonNext);
    controlsLayout->addWidget(this->buttonEnd);

    controlsBar->setLayout(controlsLayout);

    // Set widgets in layouts
    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addWidget(this->imageView, 0, 0);
    mainLayout->addWidget(this->controlsBar, 1, 0);
    mainLayout->addWidget(this->drawBar, 0, 1, 2, 1);
    mainLayout->addWidget(this->thumbnailsList,2, 0, 1, 2);

    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);

    // Final stuff
    this->setCursor(QCursor(QPixmap("../img/cursors/base.png")));

    this->createActions();
    this->createMenus();

    this->setCentralWidget(mainWidget);
    this->setWindowTitle(tr("GerardRoto"));
    this->setMinimumSize(1050, 720);
}

void MainWindow::createActions()
{
     // File menu
     this->newProjectAction = new QAction(tr("&Nouveau projet..."), this);
     this->newProjectAction->setShortcut(Qt::CTRL + Qt::Key_N);
     connect(newProjectAction, SIGNAL(triggered()), this, SLOT(newProject()));

     this->openAction = new QAction(tr("&Ouvrir..."), this);
     this->openAction->setShortcut(Qt::CTRL + Qt::Key_O);
     connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

     this->saveAction = new QAction(tr("&Enregister"), this);
     this->saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
     connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

     this->saveAsAction = new QAction(tr("&Enregistrer sous..."), this);
     this->saveAsAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
     connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

     this->exportDrawAction = new QAction(tr("&Exporter dessins..."), this);
     this->exportDrawAction->setShortcut(Qt::CTRL + Qt::Key_E);
     connect(exportDrawAction, SIGNAL(triggered()), this, SLOT(exportDraw()));

     this->exportDrawWithMovieAction = new QAction(tr("&Exporter le film..."), this);
     this->exportDrawWithMovieAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_E);
     connect(exportDrawWithMovieAction, SIGNAL(triggered()), this, SLOT(exportDrawWithMovie()));

     this->closeAction = new QAction(tr("&Fermer"), this);
     this->closeAction->setShortcut(Qt::CTRL + Qt::Key_W);
     connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

     this->quitAction = new QAction(tr("&Quitter"), this);
     this->quitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
     connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

     // Draw menu
     this->freeDrawAction = new QAction(tr("&Tracé libre"), this);
     connect(freeDrawAction, SIGNAL(triggered()), this, SLOT(freeDraw()));

     this->lineDrawAction = new QAction(tr("&Tracé en lignes"), this);
     connect(lineDrawAction, SIGNAL(triggered()), this, SLOT(lineDraw()));

     this->eraserAction = new QAction(tr("&Gomme"), this);
     connect(eraserAction, SIGNAL(triggered()), this, SLOT(eraser()));

     // Layers menu
     this->displayBackgroundMovieAction = new QAction(tr("&Film en fond"), this);
     this->displayBackgroundMovieAction->setCheckable(true);
     this->displayBackgroundMovieAction->setChecked(true);
     connect(displayBackgroundMovieAction, SIGNAL(triggered()), this, SLOT(displayBackgroundMovie()));

     this->onionPeelingsAction = new QAction(tr("&Pelures d'oignon"), this);
     this->onionPeelingsAction->setCheckable(true);
     this->onionPeelingsAction->setChecked(false);
     connect(onionPeelingsAction, SIGNAL(triggered()), this, SLOT(onionPeelings()));

     this->peelingsNumberAction = new QAction(tr("&Nombre de pelures..."), this);
     connect(peelingsNumberAction, SIGNAL(triggered()), this, SLOT(peelingsNumber()));

     // Goto menu
     this->backAction = new QAction(tr("&Image précédente"), this);
     this->backAction->setShortcut(Qt::Key_Left);
     connect(backAction, SIGNAL(triggered()), this, SLOT(back()));

     this->nextAction = new QAction(tr("&Image suivante"), this);
     this->nextAction->setShortcut(Qt::Key_Right);
     connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));

     this->beginAction = new QAction(tr("&Début"), this);
     this->beginAction->setShortcut(Qt::CTRL + Qt::Key_Left);
     connect(beginAction, SIGNAL(triggered()), this, SLOT(begin()));

     this->endAction = new QAction(tr("&Fin"), this);
     this->endAction->setShortcut(Qt::CTRL + Qt::Key_Right);
     connect(endAction, SIGNAL(triggered()), this, SLOT(end()));

     // Viewing menu
     this->playFromBeginningAction = new QAction(tr("&Défillement dessins"), this);
     connect(playFromBeginningAction, SIGNAL(triggered()), this, SLOT(playFromBeginning()));

     this->playWithMovieAction = new QAction(tr("&Défillement dessins avec film"), this);
     connect(playWithMovieAction, SIGNAL(triggered()), this, SLOT(playWithMovie()));

     // About menu
     this->aboutAction = new QAction(tr("&À Propos"), this);
     connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
     // File menu
     this->fileMenu = menuBar()->addMenu(tr("&Fichier"));

     this->fileMenu->addAction(newProjectAction);
     this->fileMenu->addAction(openAction);

     this->fileMenu->addSeparator();
     this->fileMenu->addAction(saveAction);
     this->fileMenu->addAction(saveAsAction);

     this->fileMenu->addSeparator();
     this->fileMenu->addAction(exportDrawAction);
     this->fileMenu->addAction(exportDrawWithMovieAction);

     this->fileMenu->addSeparator();
     this->fileMenu->addAction(closeAction);
     this->fileMenu->addAction(quitAction);

     // Draw menu
     this->drawMenu = menuBar()->addMenu(tr("&Dessin"));
     this->drawMenu->addAction(freeDrawAction);
     this->drawMenu->addAction(lineDrawAction);
     this->drawMenu->addAction(eraserAction);

     // Layers menu
     this->layersMenu = menuBar()->addMenu(tr("&Calques"));
     this->layersMenu->addAction(displayBackgroundMovieAction);
     this->layersMenu->addAction(onionPeelingsAction);

     this->layersMenu->addSeparator();
     this->layersMenu->addAction(peelingsNumberAction);

     // Goto menu
     this->gotoMenu = menuBar()->addMenu(tr("&Aller à"));
     this->gotoMenu->addAction(backAction);
     this->gotoMenu->addAction(nextAction);

     this->gotoMenu->addSeparator();
     this->gotoMenu->addAction(beginAction);
     this->gotoMenu->addAction(endAction);

     // Viewing menu
     this->viewingMenu = menuBar()->addMenu(tr("&Visionnage"));
     this->viewingMenu->addAction(playFromBeginningAction);
     this->viewingMenu->addAction(playWithMovieAction);

     // About menu
     this->aboutMenu = menuBar()->addMenu(tr("&Aide"));
     this->aboutMenu->addAction(aboutAction);
}

/************************** Menu slots ****************************/
void MainWindow::newProject(){
    NewProjectDialog* dialog = new NewProjectDialog();

    if(dialog->exec() == QDialog::Accepted){
       QMessageBox::about(this, "Nouveau projet", "Vidéo: " + dialog->getSelectedFile() + "\nFPS: " + QString::number(dialog->getSelectedFPSCount()));
    }
}

void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un projet"),
                                                     "",
                                                     tr("Files (*.gerard)"));
}

void MainWindow::save(){

}

void MainWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer le projet"),
                                                    "",
                                                    tr("Files (*.gerard)"));
}

void MainWindow::exportDraw(){

}

void MainWindow::exportDrawWithMovie(){

}

void MainWindow::close(){

}

void MainWindow::quit(){
    QApplication::quit();
}

void MainWindow::freeDraw(){
    this->dz->setTool(DrawZone::TOOL_PEN);
    this->setCursor(QCursor(QPixmap("../img/cursors/pencil.png")));
}

void MainWindow::lineDraw(){
    this->dz->setTool(DrawZone::TOOL_LINE);
    this->setCursor(QCursor(QPixmap("../img/cursors/line.png")));
}

void MainWindow::eraser(){
    this->dz->setTool(DrawZone::TOOL_RUBBER);
    this->setCursor(QCursor(QPixmap("../img/cursors/eraser.png")));
}

void MainWindow::changePenWidth(int width){
    this->dz->setPenWidth(width);
}

void MainWindow::changePenColor(QColor color){
    this->dz->setPenColor(color);
}

void MainWindow::displayBackgroundMovie(){

}

void MainWindow::onionPeelings(){

}

void MainWindow::peelingsNumber(){

}

void MainWindow::begin(){

}

void MainWindow::end(){

}

void MainWindow::back(){

}

void MainWindow::next(){

}

void MainWindow::playFromBeginning(){

}

void MainWindow::playWithMovie(){

}

void MainWindow::about(){
    QMessageBox::about(this, "À Propos de Gérard", "Developpeurs: Anthony Pena et Jérémy Bardon");
}
