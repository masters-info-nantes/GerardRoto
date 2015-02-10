#include "stackimage.h"
#include "drawzone.h"
#include "imagewidget.h"
#include <QLabel>

/*QWidget* makeImage(QString name, int scaledWidth)
{
    QLabel* img = new QLabel();
    QPixmap imgPix(name);
    imgPix = imgPix.scaledToWidth(scaledWidth);
    img->setPixmap(imgPix);
    return img;
}*/

QWidget* makeImage(QString name)
{
    QImage* img = new QImage(name);
    QWidget* w = new ImageWidget(img);
    delete img;
    return w;
}

StackImage::StackImage(QWidget *parent) :
    QWidget(parent),
    m_stackSize(0)
{
    QStackedLayout* l = new QStackedLayout(this);
    l->setStackingMode(QStackedLayout::StackAll);
    this->setLayout(l);
}

StackImage::~StackImage()
{

}

QWidget* StackImage::getTopElement()
{
    return this->layout()->itemAt(0)->widget();
}

void StackImage::push(QWidget *img)
{
    this->layout()->addWidget(img);
    m_stackSize++;
}

void StackImage::push(QLayoutItem *img)
{
    this->push(img->widget());
}

void StackImage::push(QString imgName)
{
    this->push(makeImage(imgName));
}

void StackImage::removeAll()
{
    while(!this->layout()->isEmpty())
    {
        this->layout()->removeItem(this->layout()->itemAt(0));
    }
    m_stackSize = 0;
}

void StackImage::removeBottom()
{
    if(!this->layout()->isEmpty())
    {
        m_stackSize--;
        this->layout()->removeItem(this->layout()->itemAt(m_stackSize));
    }
}

void StackImage::removeMiddle()
{

    for(int i=(m_stackSize-2);i>0;i--)
    {
        this->layout()->removeItem(this->layout()->itemAt(i));
    }
}

void StackImage::resizeEvent(QResizeEvent *event)
{
    QSize newSize(event->size());
    QWidget* back;
    for(int i=0;i<m_stackSize;i++)
    {
        this->layout()->itemAt(i)->widget()->resize(newSize);
    }
}
