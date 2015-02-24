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
    void push(QString imgName);
    QList<QLayoutItem*>* removeAll();
    QLayoutItem* removeBottom();
    QList<QLayoutItem*>* removeMiddle();
    int stackCount();

    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void resizeEvent(QResizeEvent *event);
signals:
    void mouseOver();
    void mouseOut();
public slots:
};

#endif // STACKIMAGE_H
