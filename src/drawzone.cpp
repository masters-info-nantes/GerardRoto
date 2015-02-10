#include "drawzone.h"
#include <QDebug>

DrawZone::DrawZone(int width, int height, int penWidth, QColor penColor, QWidget *parent)
    : QWidget(parent),
      m_tracer(false),
      m_back_pos(0,0),
      m_pen(penColor),
      m_image(new QImage(width,height,QImage::Format_ARGB32_Premultiplied))
{
    m_pen.setWidth(penWidth);
}

DrawZone::~DrawZone()
{
    if(m_image != 0)
        delete m_image;
}

void DrawZone::setPenWidth(int width)
{
    m_pen.setWidth(width);
}

void DrawZone::setPenColor(QColor color)
{
    m_pen.setColor(color);
}

void DrawZone::mousePressEvent(QMouseEvent *event) {
    m_tracer = true;
    m_back_pos = event->pos();
}

void DrawZone::mouseMoveEvent(QMouseEvent *event) {
    if(m_tracer)
    {
        QPainter painter(m_image);
        painter.setPen(m_pen);
        painter.drawLine(m_back_pos,event->pos());
        m_back_pos = event->pos();
        update();
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent *event) {
    m_tracer = false;
    m_back_pos = QPoint(0,0);
}

void DrawZone::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(m_pen);
    painter.drawImage(0,0,*m_image);
}

void DrawZone::resizeEvent(QResizeEvent *event) {
    QImage* tmp = m_image;
    m_image = new QImage(m_image->scaled(event->size()));
    delete tmp;
    update();
}

void DrawZone::save(QString filename)
{
    m_image->save(filename);
}
