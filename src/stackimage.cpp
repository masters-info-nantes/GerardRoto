#include "stackimage.h"
#include <QGraphicsOpacityEffect>
#include "imagewidget.h"

QWidget* makeImage(QString name)
{
    QImage* img = new QImage(name);
    QWidget* w = new ImageWidget(img);
    delete img;
    return w;
}

StackImage::StackImage(QWidget *parent) :
    QWidget(parent),
    m_stackSize(0),
    m_changeBottomOpacity(true)
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
    this->updateOpacity();
    update();
}

void StackImage::push(QLayoutItem *img)
{
    this->push(img->widget());
}

void StackImage::push(QString imgName)
{
    this->push(makeImage(imgName));
}

QList<QLayoutItem*>* StackImage::removeAll()
{
    QList<QLayoutItem*>* list(new QList<QLayoutItem*>());
    QLayoutItem* item;
    while(!this->layout()->isEmpty())
    {
        item = this->layout()->itemAt(0);
        this->layout()->removeItem(item);
        list->append(item);
    }
    m_stackSize = 0;
    update();
    return list;
}

QLayoutItem* StackImage::removeBottom()
{
    if(!this->layout()->isEmpty())
    {
        m_stackSize--;
        QLayoutItem* item(this->layout()->itemAt(m_stackSize));
        this->layout()->removeItem(item);
        this->updateOpacity();
        update();
        return item;
    }
    return 0;
}

QList<QLayoutItem*>* StackImage::removeMiddle()
{
    QLayoutItem* top(this->layout()->itemAt(0));
    QLayoutItem* bottom(this->layout()->itemAt(m_stackSize-1));
    this->layout()->removeItem(top);
    this->layout()->removeItem(bottom);
    QList<QLayoutItem*>* list(this->removeAll());
    if(top != bottom && bottom != 0) {
        this->layout()->addItem(top);
        this->layout()->addItem(bottom);
        m_stackSize = 2;
    } else if(top == bottom && top != 0) {
        this->layout()->addItem(top);
        m_stackSize = 1;
    }
    this->updateOpacity();
    update();
    return list;
}

int StackImage::stackCount()
{
    return this->m_stackSize;
}

void StackImage::changeBottomOpacity(bool active)
{
    this->m_changeBottomOpacity = active;
}

void StackImage::enterEvent(QEvent*)
{
    emit mouseOver();
}

void StackImage::leaveEvent(QEvent*)
{
    emit mouseOut();
}

void StackImage::resizeEvent(QResizeEvent *event)
{
    QSize newSize(event->size());
    for(int i=0;i<m_stackSize;i++)
    {
        this->layout()->itemAt(i)->widget()->resize(newSize);
    }
}

void StackImage::updateOpacity()
{
    int numberOfWidget = (this->m_changeBottomOpacity ? this->m_stackSize : this->m_stackSize-1);
    qreal inc(((qreal)1)/((qreal)numberOfWidget));
    qreal nextOpacity = inc;
    QGraphicsOpacityEffect* effect;
    for(int i=(numberOfWidget-1);i>0;i--)
    {
        effect = new QGraphicsOpacityEffect();
        effect->setOpacity(nextOpacity);
        this->layout()->itemAt(i)->widget()->setGraphicsEffect(effect);
        nextOpacity += inc;
    }
}
