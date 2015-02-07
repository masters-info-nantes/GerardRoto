#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QDebug>

class ColorPicker : public QWidget
{
    Q_OBJECT

private:
    QVector<QPushButton*>* colorsLabels;

public:
    explicit ColorPicker(QWidget *parent = 0);

signals:

public slots:

};

#endif // COLORPICKER_H
