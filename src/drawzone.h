#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QtGui>
#include <QWidget>

class DrawZone : public QWidget
{
    Q_OBJECT
private:
    bool m_drawingWhenLeave;
    bool m_draw;
    int m_tool;
    QPoint m_back_pos;
    QPen m_pen;
    QImage * m_image;
    QImage * m_undo;
    QImage * m_redo;
public:
    explicit DrawZone(int width, int height, int penWidth = 3, QColor penColor = QColor(0,0,0),QWidget *parent = 0);
    ~DrawZone();

    const static int TOOL_PEN = 0;
    const static int TOOL_LINE = 1;
    const static int TOOL_RUBBER = 2;

    void setTool(int tool);
    void setPenWidth(int width);
    void setPenColor(QColor color);

    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent *event);

    bool undo();
    bool redo();

    void save(QString filename);
    QImage* replaceLayer(QImage* img);
private:
    void newdo();
signals:
    void drawEvent();
public slots:
};

#endif // DRAWZONE_H
