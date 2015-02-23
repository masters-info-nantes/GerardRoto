#include "drawzone.h"
#include <QDebug>

DrawZone::DrawZone(int width, int height, int penWidth, QColor penColor, QWidget *parent)
    : QWidget(parent),
      m_drawingWhenLeave(false),
      m_draw(false),
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
        m_draw = true;
    }
    else
    {
        m_draw = false;
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

void DrawZone::enterEvent(QEvent*)
{
    if(m_drawingWhenLeave)
    {
        m_draw = true;
        m_drawingWhenLeave = false;
    }
}

void DrawZone::leaveEvent(QEvent*)
{
    if(m_draw)
    {
        m_drawingWhenLeave = true;
        m_draw = false;
    }
}

void DrawZone::mousePressEvent(QMouseEvent *event)
{
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
    case DrawZone::TOOL_RUBBER:
        m_draw = true;
        m_back_pos = event->pos();
        newdo();
        break;
    case DrawZone::TOOL_LINE:
        // nothing
        break;
    }
}

void DrawZone::mouseMoveEvent(QMouseEvent *event)
{
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
        if(m_draw)
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
        if(m_draw)
        {
            QImage alpha(m_image->alphaChannel());
            int penHalfWidth(m_pen.width()/2);
            int imageX(m_image->size().width());
            int imageY(m_image->size().height());

            int posX(event->pos().rx());
            int posY(event->pos().ry());

            int minX(posX-penHalfWidth);
            int minY(posY-penHalfWidth);
            int maxX(posX+penHalfWidth);
            int maxY(posY+penHalfWidth);

            for(int i=(minX < 0 ? 0 : minX);i<(maxX > imageX ? imageX : maxX);i++)
            {
                for(int j=(minY < 0 ? 0 : minY);j<(maxY > imageY ? imageY : maxY);j++)
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

void DrawZone::mouseReleaseEvent(QMouseEvent *event)
{
    switch(m_tool)
    {
    case DrawZone::TOOL_PEN:
    case DrawZone::TOOL_RUBBER:
        m_draw = false;
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
                newdo();
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

void DrawZone::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(m_pen);
    painter.drawImage(0,0,*m_image);
}

void DrawZone::resizeEvent(QResizeEvent *event)
{
    QImage* tmp = m_image;
    m_image = new QImage(m_image->scaled(event->size()));
    delete tmp;
    if(m_undo != 0)
    {
        QImage* tmp = m_undo;
        m_undo = new QImage(m_undo->scaled(event->size()));
        delete tmp;
    }
    if(m_redo != 0)
    {
        QImage* tmp = m_redo;
        m_redo = new QImage(m_redo->scaled(event->size()));
        delete tmp;
    }
    update();
}

void DrawZone::newdo()
{
    if(m_undo != 0)
        delete m_undo;
    m_undo = new QImage(*m_image);
    if(m_redo != 0)
        delete m_redo;
    emit drawEvent();
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

void DrawZone::save(QString filename, QSize size)
{
    QImage scaledDraw(m_image->scaled(size));
    scaledDraw.save(filename);
}

QImage* DrawZone::replaceLayer(QImage* img)
{
    QImage* tmp = m_image;
    m_image = new QImage(img->scaled(tmp->size()));
    m_undo = 0;
    m_redo = 0;
    update();
    return tmp;
}
