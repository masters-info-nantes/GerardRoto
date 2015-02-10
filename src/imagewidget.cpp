#include "imagewidget.h"

ImageWidget::ImageWidget(QImage *img, QWidget *parent) :
    m_origin(new QImage(*img)),
    m_image(new QImage(*img)),
    QWidget(parent)
{
}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::black);
    painter.setPen(pen);
    painter.drawImage(0,0,*m_image);
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    delete m_image;
    m_image = new QImage(m_origin->scaled(event->size()));
    update();
}
