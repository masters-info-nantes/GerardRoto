#ifndef SEQUENCEWIDGET_H
#define SEQUENCEWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QHBoxLayout>

class SequenceWidget : public QWidget
{
    Q_OBJECT
private:
    int m_time;
    QVector<QWidget*>* m_list;
    QTimer* m_timer;
    int m_currentImage;
public:
    explicit SequenceWidget(QWidget *parent = 0);
    ~SequenceWidget();

    void addWidget(QWidget* w);
    QWidget *removeLast();
    QVector<QWidget*>* removeAll();

    void setTime(int millis);
    void start();
signals:
    void sequenceEnd();
public slots:
    void changeWidget();
};

#endif // SEQUENCEWIDGET_H
