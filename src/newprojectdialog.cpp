#include "newprojectdialog.h"

NewProjectDialog::NewProjectDialog()
    : QDialog()
{
    QHBoxLayout* fileLayout = new QHBoxLayout();
    fileLayout->addWidget(new QLabel(tr("Fichier:")));

    this->editFile = new QLineEdit(tr("Aucun fichier sélectionné"));
    this->editFile->setEnabled(false);
    this->editFile->setMinimumWidth(180);

    fileLayout->addWidget(this->editFile);

    QPushButton* buttonBrowse = new QPushButton(tr("Parcourir..."));
    connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(selectFile()));

    fileLayout->addWidget(buttonBrowse);

    groupFPS = new QButtonGroup();
    QHBoxLayout* layoutFPS = new QHBoxLayout();

    for(int i = 6; i < 20; i+=4){
        QRadioButton* radio = new QRadioButton(QString::number(i));
        groupFPS->addButton(radio);
        layoutFPS->addWidget(radio);
    }

    groupFPS->setExclusive(true);
    groupFPS->buttons().at(0)->setChecked(true);

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    this->buttonOpen = new QPushButton(tr("Créer"));
    connect(this->buttonOpen, SIGNAL(clicked()), this, SLOT(accept()));
    buttonBox->addButton(this->buttonOpen, QDialogButtonBox::AcceptRole);

    this->buttonCancel = new QPushButton(tr("Annuler"));
    connect(this->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    buttonBox->addButton(this->buttonCancel, QDialogButtonBox::RejectRole);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(fileLayout);
    mainLayout->addWidget(new QLabel(tr("Nombre d'images par secondes: ")));
    mainLayout->addLayout(layoutFPS);
    mainLayout->addWidget(buttonBox);

    this->setWindowTitle(tr("Nouveau projet"));
    this->setLayout(mainLayout);
}

QString NewProjectDialog::getSelectedFile(){
    return this->editFile->text();
}

int NewProjectDialog::getSelectedFPSCount(){
    return this->groupFPS->checkedButton()->text().toInt();
}

void NewProjectDialog::selectFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une vidéo"), "", "");
    this->editFile->setText(fileName);
}

