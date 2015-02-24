#include "mainwindow.h"
#include "drawzone.h"

QString* drawImageName(QString s)
{
    QFileInfo pictName(s);
    QString *drawName(new QString(pictName.absolutePath() + "/" + pictName.completeBaseName() + ".draw" + ".png"));

    return drawName;
}

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
      perspective(false),
      currentIndex(-1),
      backgroundDisplayed(true),
      onionDisplayed(false),
      peelingsCount(DEFAULT_PEELINGS_COUNT),
      projectFullPath(""),
      allDrawSaved(true)
{
    // Create widgets
    this->imageView = new StackImage();
    drawzone = new DrawZone(1000,600);

    this->thumbnailsList = new QListWidget();
    this->thumbnailsList->setFixedHeight(130);
    this->thumbnailsList->setFlow(QListView::LeftToRight);
    //this->thumbnailsList->setSpacing(6);
    connect(this->thumbnailsList, SIGNAL(currentRowChanged(int)), this, SLOT(thumbClick(int)));

    QGridLayout* drawLayout = new QGridLayout();
    this->buttonFreeDraw = new QPushButton();
    connect(this->buttonFreeDraw, SIGNAL(clicked()), this, SLOT(freeDraw()));
    this->buttonFreeDraw->setIcon(QIcon(QPixmap(":icons/img/icons/pencil.png")));
    this->buttonFreeDraw->setCheckable(true);
    this->buttonFreeDraw->setChecked(true);

    this->buttonLineDraw = new QPushButton();
    connect(this->buttonLineDraw, SIGNAL(clicked()), this, SLOT(lineDraw()));
    this->buttonLineDraw->setIcon(QIcon(QPixmap(":icons/img/icons/line.png")));
    this->buttonLineDraw->setCheckable(true);

    this->buttonEraser = new QPushButton();
    connect(this->buttonEraser, SIGNAL(clicked()), this, SLOT(eraser()));
    this->buttonEraser->setIcon(QIcon(QPixmap(":icons/img/icons/eraser.png")));
    this->buttonEraser->setCheckable(true);

    QButtonGroup* drawButtonGroup = new QButtonGroup();
    drawButtonGroup->addButton(this->buttonFreeDraw);
    drawButtonGroup->addButton(this->buttonLineDraw);
    drawButtonGroup->addButton(this->buttonEraser);
    drawButtonGroup->setExclusive(true);

    this->numberBrushSize = new QSpinBox();
    connect(this->numberBrushSize, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
    this->numberBrushSize->setValue(8);

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
    this->buttonBegin->setIcon(QIcon(QPixmap(":icons/img/icons/begin.png")));
    connect(this->buttonBegin, SIGNAL(clicked()), this, SLOT(begin()));

    this->buttonEnd = new QPushButton();
    this->buttonEnd->setIcon(QIcon(QPixmap(":icons/img/icons/end.png")));
    connect(this->buttonEnd, SIGNAL(clicked()), this, SLOT(end()));

    this->buttonBack = new QPushButton();
    this->buttonBack->setIcon(QIcon(QPixmap(":icons/img/icons/backward.png")));
    connect(this->buttonBack, SIGNAL(clicked()), this, SLOT(back()));

    this->buttonNext = new QPushButton();
    this->buttonNext->setIcon(QIcon(QPixmap(":icons/img/icons/forward.png")));
    connect(this->buttonNext, SIGNAL(clicked()), this, SLOT(next()));

    this->frameNumber = new QLineEdit();
    connect(this->frameNumber, SIGNAL(returnPressed()), this, SLOT(goFrame()));
    this->buttonGoFrame = new QPushButton("Go");
    connect(this->buttonGoFrame, SIGNAL(clicked()), this, SLOT(goFrame()));

    this->numberPreviousFrames = new QSpinBox();

    this->buttonPlayDraws = new QPushButton();
    this->buttonPlayDraws->setIcon(QIcon(QPixmap(":icons/img/icons/play.png")));

    this->labelWithMovie = new QLabel();
    this->labelWithMovie->setPixmap(QPixmap(":icons/img/icons/movie.png").scaledToHeight(20));

    this->checkPlayWithMovie = new QCheckBox();
    this->buttonPlayFull = new QPushButton("Depuis le début");
    this->buttonPlayFull->setIcon(QIcon(QPixmap(":icons/img/icons/play.png")));

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
    this->createActions();
    this->createMenus();

    cursors[CURSOR_BASE] = new QCursor(QPixmap(":cursors/img/cursors/base.png"));
    cursors[CURSOR_FREE] = new QCursor(QPixmap(":cursors/img/cursors/pencil.png"));
    cursors[CURSOR_LINE] = new QCursor(QPixmap(":cursors/img/cursors/line.png"));
    cursors[CURSOR_ERASER] = new QCursor(QPixmap(":cursors/img/cursors/eraser.png"));
    toolCursor = cursors[CURSOR_FREE];
    this->setCursor(*cursors[CURSOR_BASE]);

    connect(imageView, SIGNAL(mouseOver()), this, SLOT(mouseEnterDrawZone()));
    connect(imageView, SIGNAL(mouseOut()), this, SLOT(mouseLeaveDrawZone()));


    this->setCentralWidget(mainWidget);
    this->setWindowTitle(tr("GerardRoto"));
    this->setMinimumSize(1050, 720);
    this->setPerspective(true);
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
     this->undoAction = new QAction(tr("&Annuler"), this);
     this->undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
     connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

     this->redoAction = new QAction(tr("&Refaire"), this);
     this->redoAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
     connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));


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
     connect(displayBackgroundMovieAction, SIGNAL(toggled(bool)), this, SLOT(displayBackgroundMovie(bool)));

     this->onionPeelingsAction = new QAction(tr("&Pelures d'oignon"), this);
     this->onionPeelingsAction->setCheckable(true);
     this->onionPeelingsAction->setChecked(false);
     connect(onionPeelingsAction, SIGNAL(toggled(bool)), this, SLOT(onionPeelings(bool)));

     this->peelingsNumberAction = new QAction(tr("&Nombre de pelures..."), this);
     connect(peelingsNumberAction, SIGNAL(triggered()), this, SLOT(peelingsNumber()));

     // Goto menu
     this->backAction = new QAction(tr("&Image précédente"), this);
     this->backAction->setShortcut(Qt::CTRL+ Qt::Key_Left);
     connect(backAction, SIGNAL(triggered()), this, SLOT(back()));

     this->nextAction = new QAction(tr("&Image suivante"), this);
     this->nextAction->setShortcut(Qt::CTRL + Qt::Key_Right);
     connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));

     this->beginAction = new QAction(tr("&Début"), this);
     this->beginAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Left);
     connect(beginAction, SIGNAL(triggered()), this, SLOT(begin()));

     this->endAction = new QAction(tr("&Fin"), this);
     this->endAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Right);
     connect(endAction, SIGNAL(triggered()), this, SLOT(end()));

     // Viewing menu
     this->playFromBeginningAction = new QAction(tr("&Défillement dessins"), this);
     connect(playFromBeginningAction, SIGNAL(triggered()), this, SLOT(playFromBeginning()));

     this->playWithMovieAction = new QAction(tr("&Défillement dessins avec film"), this);
     connect(playWithMovieAction, SIGNAL(triggered()), this, SLOT(playWithMovie()));

     // About menu
     this->aboutAction = new QAction(tr("&À Propos"), this);
     connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

     connect(this->drawzone, SIGNAL(drawEvent()), this, SLOT(drawZoneNewDraw()));
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
     this->drawMenu->addAction(undoAction);
     this->drawMenu->addAction(redoAction);

     this->drawMenu->addSeparator();
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

// Loads thumbnails with images in working directory
// Not use draw images that ends with ".draw.png"
void MainWindow::updateThumbnails(){

    QDir dir(this->workingDir->path());
    QStringList files(dir.entryList());

    for(int i = 0; i < files.length(); i++){
        QString file(files.at(i));
        if(file == "." || file == ".." || file.endsWith(".draw.png")){
            continue;
        }

        QLabel* thumbLabel = new QLabel();
        thumbLabel->setAlignment(Qt::AlignCenter);
        thumbLabel->setToolTip(this->workingDir->path() + "/" + file);

        QPixmap* thumbFull = new QPixmap(this->workingDir->path() + "/" + file);
        QPixmap thumbScaled(thumbFull->scaledToHeight(100));

        thumbLabel->setPixmap(thumbScaled);
        thumbLabel->setMinimumSize(QSize(thumbScaled.width(), thumbScaled.height()));

        QListWidgetItem* listItem = new QListWidgetItem(this->thumbnailsList);
        listItem->setSizeHint(QSize(thumbLabel->minimumWidth()+ 30, thumbLabel->minimumHeight()));

        this->thumbnailsList->addItem(listItem);
        this->thumbnailsList->setItemWidget(listItem, thumbLabel);

        delete thumbFull;// not use after
    }
}

// Change the current image
// - saves draw layer
// - loads next image and draw layer
void MainWindow::changeCurrentImage(int index){

    int maxIndex(this->thumbnailsList->count() - 1);
    if(index < 0) index = 0;
    else if(index > maxIndex) index = maxIndex;

    // Save current draw layer
    // Caution: When the first picture is set (project load)
    // it erase the saved draw with the default draw layer
    if(this->currentIndex >= 0){
        this->saveCurrentDraw();
    }

    // Load next draw layer
    this->thumbnailsList->setCurrentRow(index);
    this->frameNumber->setText(QString::number(index + 1));

    QLabel* label((QLabel*)this->thumbnailsList->itemWidget(this->thumbnailsList->item(index)));
    QString drawName(drawImageName(label->toolTip())->constData());

    // Replace draw zone layer (empty if not exists)
    QFile nextDrawFile(drawName);
    QImage *back, *img;
    if(nextDrawFile.exists()){
        img = new QImage(drawName);
        back = this->drawzone->replaceLayer(img);
    }
    else
    {
        img = new QImage(this->drawzone->size(), QImage::Format_ARGB32);
        back = this->drawzone->replaceLayer(img);
    }

    if(this->backgroundDisplayed)
    {
        QLayoutItem* previousBackground(this->imageView->removeBottom());
        delete previousBackground;
    }

    this->imageView->removeAll();
    this->imageView->push(this->drawzone);

    if(this->backgroundDisplayed){
        this->imageView->push(label->toolTip());
    }

    if(this->onionDisplayed)
    {
        this->onionDisplayed = false;
        this->onionPeelings(true);
    }
    this->currentIndex = index;
    delete img;
    delete back;
}

// Save current draw layer
void MainWindow::saveCurrentDraw(){
    QLabel* label = (QLabel*)this->thumbnailsList->itemWidget(this->thumbnailsList->item(this->currentIndex));
    if(label == NULL) return;

    QFileInfo pictName(QFileInfo(label->toolTip()));
    QString drawName(pictName.absolutePath() + "/" + pictName.completeBaseName() + ".draw" + ".png");

    this->drawzone->save(drawName, QPixmap(label->toolTip()).size());
    qDebug() << drawName + " saved";
}

// Change perpective between no project opened and
// project openend
void MainWindow::setPerspective(bool noProject){
    if(this->perspective != noProject)
    {
        this->perspective = noProject;

        this->controlsBar->setDisabled(noProject);
        this->drawBar->setDisabled(noProject);
        this->thumbnailsList->setDisabled(noProject);

        this->saveAction->setDisabled(noProject);
        this->saveAsAction->setDisabled(noProject);
        this->exportDrawAction->setDisabled(noProject);
        this->exportDrawWithMovieAction->setDisabled(noProject);
        this->closeAction->setDisabled(noProject);

        this->drawMenu->setDisabled(noProject);
        this->layersMenu->setDisabled(noProject);
        this->gotoMenu->setDisabled(noProject);
        this->viewingMenu->setDisabled(noProject);
    }
}

void MainWindow::notSavedIndication(bool display)
{
    if(display)
    {
        this->setWindowTitle("* " + this->windowTitle());
    }
    else
    {
        QString currentTitle(this->windowTitle());
        this->setWindowTitle(currentTitle.right(currentTitle.size()-2));
    }
}

bool MainWindow::askForSaving(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                    "Sauvegarder le projet courant avant de le fermer ?",
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    switch(reply){
        case QMessageBox::Yes:
            return true;
        default:
            return false;
    }
}

/************************** Menu slots ****************************/
void MainWindow::newProject(){
    NewProjectDialog* dialog = new NewProjectDialog();

    if(dialog->exec() == QDialog::Accepted){

       if(this->projectFullPath != ""){
            this->close();
       }

       QFileInfo selectedFile(QFileInfo(dialog->getSelectedFile()));

       this->projectName = selectedFile.baseName();
       this->workingDir = new QTemporaryDir();
       this->projectFullPath = "";

       qDebug() << this->workingDir->path();

       QStringList args;
       args << "-i" << dialog->getSelectedFile();
       args << "-r" << QString::number(dialog->getSelectedFPSCount());
       args << selectedFile.baseName() + "-%3d.jpeg";

       //QProcess::execute("ffmpeg", args);
       QProcess command;
       command.setWorkingDirectory(this->workingDir->path());
       command.start("ffmpeg", args);
       command.waitForFinished();

       this->setWindowTitle("* GerardRoto - " + this->projectName);
       this->allDrawSaved = false;

       this->updateThumbnails();
       this->changeCurrentImage(0);
       this->setPerspective(false);
    }
}

void MainWindow::open(){
    QString projectFile = QFileDialog::getOpenFileName(this, tr("Ouvrir un projet"),
                                                     "",
                                                     tr("Files (*.gerard)"));

    if(projectFile == NULL){
        return; // user canceled window
    }
    else if(this->projectFullPath != ""){
        this->close();
    }

    this->projectFullPath = projectFile;

    QFileInfo selectedFile(QFileInfo(this->projectFullPath));
    this->projectName = selectedFile.baseName();
    this->workingDir = new QTemporaryDir();

    qDebug() << this->workingDir->path();
    QStringList args;
    args << "-xvf" << this->projectFullPath;
    args << "-C" << this->workingDir->path();

    QProcess command;
    command.setWorkingDirectory(this->workingDir->path());
    command.start("tar", args);
    command.waitForFinished();

    this->setWindowTitle("GerardRoto - " + this->projectName);
    this->updateThumbnails();
    this->changeCurrentImage(0);
    this->setPerspective(false);
}

void MainWindow::save(){

    if(this->projectFullPath == ""){
        this->saveAs();
        return;
    }

    this->saveCurrentDraw();

    if(!this->projectFullPath.endsWith(".gerard")){
        this->projectFullPath += ".gerard";
    }

    QFile projectFile(this->projectFullPath);
    if(projectFile.exists()){
        projectFile.remove();
    }

    QStringList args;
    args << "-cvf" << this->projectFullPath;

    QDir dir(this->workingDir->path());
    QStringList files(dir.entryList());

    for(int i = 0; i < files.length(); i++){
        QString file(files.at(i));
        if(file != "." && file != ".."){
            args << file;
        }
    }

    QProcess command;
    command.setWorkingDirectory(this->workingDir->path());
    command.start("tar", args);
    command.waitForFinished();

    if(!this->allDrawSaved)
    {
        this->allDrawSaved = true;
        this->notSavedIndication(false);
    }

}

void MainWindow::saveAs(){
    this->projectFullPath = QFileDialog::getSaveFileName(this, tr("Enregistrer le projet"),
                                                    this->projectName,
                                                    tr("Files (*.gerard)"));

    if(this->projectFullPath == NULL){
        return; // user canceled window
    }

    this->save();
}

void MainWindow::exportDraw(){
    QString drawArchive(QFileDialog::getSaveFileName(this, tr("Export des dessins"),
                                                    this->projectName + "-draws",
                                                    tr("Files (*.tar)")));

    this->saveCurrentDraw();

    if(!drawArchive.endsWith(".tar")){
        drawArchive += ".tar";
    }

    QFile exportFile(drawArchive);
    if(exportFile.exists()){
        exportFile.remove();
    }

    QStringList args;
    args << "-cvf" << drawArchive;

    QDir dir(this->workingDir->path());
    QStringList files(dir.entryList());

    for(int i = 0; i < files.length(); i++){
        QString file (files.at(i));
        if(file != "." && file != ".." && file.endsWith(".draw.png")){
            args << file;
        }
    }

    QProcess command;
    command.setWorkingDirectory(this->workingDir->path());
    command.start("tar", args);
    command.waitForFinished();
}

void MainWindow::exportDrawWithMovie(){
    QString drawArchive(QFileDialog::getSaveFileName(this, tr("Export des dessins avec le film"),
                                                    this->projectName + "-drawmovie",
                                                    tr("Files (*.tar)")));

    this->saveCurrentDraw();

    if(!drawArchive.endsWith(".tar")){
        drawArchive += ".tar";
    }

    QFile exportFile(drawArchive);
    if(exportFile.exists()){
        exportFile.remove();
    }

    QStringList args;
    args << "-cvf" << drawArchive;

    QDir dir(this->workingDir->path());
    QStringList files(dir.entryList());

    QDir().mkdir(this->workingDir->path() + "/" + "export");
    for(int i = 0; i < files.length(); i++){
        QString file(files.at(i));
        if(file != "." && file != ".." && file.endsWith(".draw.png")){
            QFileInfo drawFile(file);

            QImage drawPic(this->workingDir->path() + "/" + file), moviePic(this->workingDir->path() + "/" + drawFile.baseName() + ".jpeg");
            QImage result(moviePic.size() ,QImage::Format_RGB32);

            QPainter painter;
            painter.begin(&result);
            painter.drawImage(0, 0, moviePic);
            painter.drawImage(0, 0, drawPic);
            painter.end();

            QString multiName(this->workingDir->path() + "/export/" + drawFile.baseName() + ".multi.jpeg");
            result.save(multiName);

            args << multiName;
        }
    }

    QProcess command;
    command.setWorkingDirectory(this->workingDir->path());
    command.start("tar", args);
    command.waitForFinished();

    QDir(this->workingDir->path() + "/export").removeRecursively();
}

void MainWindow::close(){
    if(!this->allDrawSaved && this->askForSaving()){
        this->save();
    }
    this->allDrawSaved = true; // anyway

    // CAUTION: launches currentRowChanged twice on the same thumb....
    this->thumbnailsList->clear();

    delete this->workingDir;
    delete this->drawzone->clear();
    this->imageView->removeAll();

    this->currentIndex = -1;
    this->setWindowTitle("GerardRoto");
    this->setPerspective(true);
    this->mouseLeaveDrawZone();
}

void MainWindow::quit(){
    this->close();
    QApplication::quit();
}

void MainWindow::undo(){
    this->drawzone->undo();
}

void MainWindow::redo(){
    this->drawzone->redo();
}

void MainWindow::freeDraw(){
    this->drawzone->setTool(DrawZone::TOOL_PEN);
    toolCursor = cursors[CURSOR_FREE];
}

void MainWindow::lineDraw(){
    this->drawzone->setTool(DrawZone::TOOL_LINE);
    toolCursor = cursors[CURSOR_LINE];
}

void MainWindow::eraser(){
    this->drawzone->setTool(DrawZone::TOOL_RUBBER);
    toolCursor = cursors[CURSOR_ERASER];
}

void MainWindow::changePenWidth(int width){
    this->drawzone->setPenWidth(width);
}

void MainWindow::changePenColor(QColor color){
    this->drawzone->setPenColor(color);
}

void MainWindow::displayBackgroundMovie(bool active){
    if(this->backgroundDisplayed != active)
    {
        if(active)
        {
            int index = this->thumbnailsList->currentRow();
            QString backgroundName = ((QLabel*)this->thumbnailsList->itemWidget(this->thumbnailsList->item(index)))->toolTip();
            this->imageView->push(backgroundName);
        }
        else
        {
            this->imageView->removeBottom();
        }
        this->backgroundDisplayed = active;
    }
}

void MainWindow::onionPeelings(bool active){
    if(this->onionDisplayed != active)
    {
        if(active)
        {
            int index = this->thumbnailsList->currentRow();

            bool backgroundDisplayedBefore(this->backgroundDisplayed);
            if(backgroundDisplayedBefore){
                this->displayBackgroundMovie(false);
            }

            int min(index-this->peelingsCount);
            if(min < 0){
                min = 0;
            }

            for(int i=index-1;i>=min;i--)
            {
                QString imageName(((QLabel*)this->thumbnailsList->itemWidget(this->thumbnailsList->item(i)))->toolTip());
                QString drawImage(drawImageName(imageName)->constData());

                if(QFile(drawImage).exists()){
                    this->imageView->push(drawImage);
                }
            }

            if(backgroundDisplayedBefore){
                this->displayBackgroundMovie(true);
            }
        }
        else
        {
            this->imageView->removeMiddle();

            if(!this->backgroundDisplayed){
                this->imageView->removeBottom();
            }
        }
        this->onionDisplayed = active;
    }
}

void MainWindow::peelingsNumber(){
    this->peelingsCount = QInputDialog::getInt(this, "Pelures d'oignons", "Nombre de pelures à afficher", this->peelingsCount, 0, 5);

    if(this->onionDisplayed){
        this->onionPeelings(false);
        this->onionPeelings(true);
    }
}

void MainWindow::back(){
    int backIndex = this->thumbnailsList->currentRow() - 1;
    this->changeCurrentImage(backIndex);
}

void MainWindow::next(){
    int nextIndex = this->thumbnailsList->currentRow() + 1;
    this->changeCurrentImage(nextIndex);
}

void MainWindow::begin(){
    this->changeCurrentImage(0);
}

void MainWindow::end(){
    this->changeCurrentImage(this->thumbnailsList->count() - 1);
}

void MainWindow::goFrame(){
    this->changeCurrentImage(this->frameNumber->text().toInt() - 1);
}

void MainWindow::playFromBeginning(){
    // TODO MainWindow::playFromBeginning()
}

void MainWindow::playWithMovie(){
    // TODO MainWindow::playWithMovie()
}

void MainWindow::about(){
    QMessageBox::about(this, "À Propos de Gérard", "Developpeurs: Anthony Pena et Jérémy Bardon");
}

void MainWindow::mouseEnterDrawZone()
{
    if(!this->perspective)
        this->setCursor(*toolCursor);
}

void MainWindow::mouseLeaveDrawZone()
{
    this->setCursor(*cursors[CURSOR_BASE]);
}

void MainWindow::drawZoneNewDraw()
{
    if(allDrawSaved)
    {
        allDrawSaved = false;
        notSavedIndication(true);
    }
}

void MainWindow::thumbClick(int index){
    this->changeCurrentImage(index);
}
