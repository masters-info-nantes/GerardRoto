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
            "QPushButton {background-color: " + colorList.at(i) + "; border: 2px solid white;}" +
            "QPushButton:checked {background-color: " + colorList.at(i) + "; border: 3px double #444444}"
        );

        currentButton->setToolTip(colorList.at(i));
        connect(currentButton, SIGNAL(clicked()), this, SLOT(buttonColorClicked()));

        colorGroup->addButton(currentButton);

        layout->addWidget(currentButton, i/2, i%2);
        this->colorsLabels->push_back(currentButton);
    }

    this->currentColor = new QColor("black");
    this->colorsLabels->at(0)->setChecked(true);

    colorGroup->setExclusive(true);
    this->setLayout(layout);
}

void ColorPicker::buttonColorClicked(){
    QPushButton* emitter = (QPushButton*) QObject::sender();
    this->currentColor = new QColor(emitter->toolTip());
    emit colorChanged(*this->currentColor);
}
