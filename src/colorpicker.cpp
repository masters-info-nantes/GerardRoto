#include "colorpicker.h"

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent)
{
    QStringList colorList;
    colorList << "black" << "white" << "gray" << "blue"
              << "green" << "yellow" << "orange" << "red";

    QGridLayout* layout = new QGridLayout();
    layout->setMargin(0);
    layout->setContentsMargins(10, 10, 10, 10);

    QButtonGroup* colorGroup = new QButtonGroup();
    this->colorsLabels = new QVector<QPushButton*>();
    for(int i = 0; i < 8; i++){
        QPushButton* currentButton = new QPushButton();
        currentButton->setAutoFillBackground(true);
        currentButton->setFixedHeight(30);
        currentButton->setCheckable(true);
        currentButton->setStyleSheet(
            "QPushButton {background-color: " + colorList.at(i) + "; border: 6px solid white;}" +
            "QPushButton:checked {background-color: " + colorList.at(i) + "; border: 1px solid white}"
        );

        colorGroup->addButton(currentButton);

        layout->addWidget(currentButton, i/2, i%2);
        this->colorsLabels->push_back(currentButton);
    }

    colorGroup->setExclusive(true);
    this->setLayout(layout);
}
