#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QtGui>

class ImageWidget : public QWidget
{
    Q_OBJECT
private:
    QImage* m_origin;
    QImage* m_image;
public:
    explicit ImageWidget(QImage* img, QWidget *parent = 0);
    ~ImageWidget();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:

public slots:
};

#endif // IMAGEWIDGET_H
