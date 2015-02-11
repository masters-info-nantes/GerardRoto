#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLineEdit>
#include <QFileDialog>

class NewProjectDialog : public QDialog
{
    Q_OBJECT

private:
    QPushButton* buttonOpen;
    QPushButton* buttonCancel;
    QLineEdit* editFile;
    QButtonGroup* groupFPS;

public:
    NewProjectDialog();
    QString getSelectedFile();
    int getSelectedFPSCount();

signals:

private slots:
    void selectFile();
};

#endif // NEWPROJECTDIALOG_H
