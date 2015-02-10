#ifndef STACKIMAGE_H
#define STACKIMAGE_H

#include <QWidget>
#include <QStackedLayout>

class StackImage : public QWidget
{
    Q_OBJECT
private:
    int m_stackSize;
public:
    explicit StackImage(QWidget *parent = 0);
    ~StackImage();

    QWidget* getTopElement();
    void push(QWidget* img);
    void push(QLayoutItem* img);
    void push(QString imgName, int scaledHeight);
    void removeAll();
    void removeBottom();
    void removeMiddle();
    void resizeEvent(QResizeEvent *event);
signals:

public slots:
};

#endif // STACKIMAGE_H
