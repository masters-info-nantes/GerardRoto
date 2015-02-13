#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QColor>
#include <qdebug.h>

class ColorPicker : public QWidget
{
    Q_OBJECT

private:
    QVector<QPushButton*>* colorsLabels;
    QColor* currentColor;

public:
    explicit ColorPicker(QWidget *parent = 0);

signals:
    void colorChanged(QColor);

private slots:
    void buttonColorClicked();
};

#endif // COLORPICKER_H
