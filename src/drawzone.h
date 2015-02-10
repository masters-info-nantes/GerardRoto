#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QtGui>
#include <QWidget>

class DrawZone : public QWidget
{
    Q_OBJECT
private:
    bool m_tracer;
    QPoint m_back_pos;
    QPen m_pen;
    QImage * m_image;
public:
    explicit DrawZone(int width, int height, int penWidth = 3, QColor penColor = QColor(0,0,0),QWidget *parent = 0);
    ~DrawZone();

    void setPenWidth(int width);
    void setPenColor(QColor color);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void save(QString filename);
signals:

public slots:
};

#endif // DRAWZONE_H
