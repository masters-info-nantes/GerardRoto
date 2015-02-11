#include "drawzone.h"
#include <QDebug>

DrawZone::DrawZone(int width, int height, int penWidth, QColor penColor, QWidget *parent)
    : QWidget(parent),
      m_tracer(false),
      m_tool(0),
      m_back_pos(0,0),
      m_pen(penColor),
      m_image(new QImage(width,height,QImage::Format_ARGB32_Premultiplied))
{
    m_pen.setWidth(penWidth);
    this->setTool(DrawZone::TOOL_LINE);
}

DrawZone::~DrawZone()
{
    if(m_image != 0)
        delete m_image;
}

void DrawZone::setTool(int tool)
{
    m_tool = tool;
    if(m_tool == TOOL_LINE)
    {
        m_tracer = true;
    }
    else
    {
        m_tracer = false;
        m_back_pos = QPoint(0,0);
    }
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
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
        m_tracer = true;
        m_back_pos = event->pos();
        break;
    case DrawZone::TOOL_LINE:
        // nothing
        break;
    case DrawZone::TOOL_RUBBER:
        break;
    }
}

void DrawZone::mouseMoveEvent(QMouseEvent *event) {
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
        if(m_tracer)
        {
            QPainter painter(m_image);
            painter.setPen(m_pen);
            painter.drawLine(m_back_pos,event->pos());
            m_back_pos = event->pos();
            update();
        }
        break;
    case DrawZone::TOOL_LINE:
        // nothing
        break;
    case DrawZone::TOOL_RUBBER:
        break;
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent *event) {
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
        m_tracer = false;
        m_back_pos = QPoint(0,0);
        break;
    case DrawZone::TOOL_LINE:
        if(m_back_pos != (QPoint(0,0)))
        {
            QPainter painter(m_image);
            painter.setPen(m_pen);
            painter.drawLine(m_back_pos,event->pos());
        }
        m_back_pos = event->pos();
        update();
        break;
    case DrawZone::TOOL_RUBBER:
        break;
    }
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
