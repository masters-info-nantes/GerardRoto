#include "drawzone.h"
#include <QDebug>

DrawZone::DrawZone(int width, int height, int penWidth, QColor penColor, QWidget *parent)
    : QWidget(parent),
      m_tracer(false),
      m_tool(DrawZone::TOOL_PEN),
      m_back_pos(0,0),
      m_pen(penColor),
      m_image(new QImage(width,height,QImage::Format_ARGB32)),
      m_undo(0),
      m_redo(0)
{
    m_pen.setWidth(penWidth);
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
    case DrawZone::TOOL_RUBBER:
        m_tracer = true;
        m_back_pos = event->pos();
        if(m_undo != 0)
            delete m_undo;
        m_undo = new QImage(*m_image);
        break;
    case DrawZone::TOOL_LINE:
        // nothing
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
        //nothing
        break;
    case DrawZone::TOOL_RUBBER:
        if(m_tracer)
        {
            QImage alpha(m_image->alphaChannel());
            int penHalfWidth = m_pen.width()/2;
            int posX = event->pos().rx();
            int posY = event->pos().ry();
            int minX = posX-penHalfWidth;
            int minY = posY-penHalfWidth;
            int maxX = posX+penHalfWidth;
            int maxY = posY+penHalfWidth;

            for(int i=minX;i<maxX;i++)
            {
                for(int j=minY;j<maxY;j++)
                {
                    alpha.setPixel(i,j,0);
                }
            }

            m_back_pos = event->pos();
            m_image->setAlphaChannel(alpha);
            update();
        }
        break;
    }
}

void DrawZone::mouseReleaseEvent(QMouseEvent *event) {
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
    case DrawZone::TOOL_RUBBER:
        m_tracer = false;
        m_back_pos = QPoint(0,0);
        break;
    case DrawZone::TOOL_LINE:
        if(event->button() == Qt::RightButton)
        {
            m_back_pos = QPoint(0,0);
        }
        else
        {
            if(m_back_pos != (QPoint(0,0)))
            {
                if(m_undo != 0)
                    delete m_undo;
                m_undo = new QImage(*m_image);
                QPainter painter(m_image);
                painter.setPen(m_pen);
                painter.drawLine(m_back_pos,event->pos());
            }
            m_back_pos = event->pos();
            update();
        }
        break;
    }
}

void DrawZone::paintEvent(QPaintEvent*) {
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

bool DrawZone::undo()
{
    if(m_undo != 0)
    {
        m_redo = m_image;
        m_image = m_undo;
        m_undo = 0;
        update();
        return true;
    }
    return false;
}

bool DrawZone::redo()
{
    if(m_redo != 0)
    {
        m_undo = m_image;
        m_image = m_redo;
        m_redo = 0;
        update();
        return true;
    }
    return false;
}

void DrawZone::save(QString filename)
{
    m_image->save(filename);
}
