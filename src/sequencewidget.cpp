#include "sequencewidget.h"

SequenceWidget::SequenceWidget(QWidget *parent)
    :QWidget(parent),
      m_time(0),
      m_list(new QVector<QWidget*>()),
      m_timer(new QTimer(this)),
      m_currentImage(-1)
{
    QHBoxLayout* l = new QHBoxLayout();
    this->setLayout(l);
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(changeWidget()));
}

SequenceWidget::~SequenceWidget()
{
}

void SequenceWidget::addWidget(QWidget* w)
{
    this->m_list->append(w);
}

QWidget* SequenceWidget::removeLast()
{
    QWidget* removed(this->m_list->last());
    this->m_list->removeLast();
    return removed;
}

QVector<QWidget*>* SequenceWidget::removeAll()
{
    /*if(this->m_list.size() > 0)
        this->layout()->removeItem(this->layout()->itemAt(0));
    m_list.clear();*/
    QVector<QWidget*>* prev = this->m_list;
    this->m_list = new QVector<QWidget*>();
    return prev;
}


void SequenceWidget::setTime(int millis)
{
    this->m_time = millis;
}

void SequenceWidget::start()
{
    m_timer->start(m_time);
}

void SequenceWidget::changeWidget()
{
    if (this->layout() != NULL)
    {
        QLayoutItem* item;
        while((item = this->layout()->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    if(this->m_currentImage < this->m_list->size()-1)
    {

        this->m_currentImage++;
        this->layout()->addWidget(this->m_list->at(this->m_currentImage));
    }
    else
    {
        this->m_timer->stop();
        this->m_currentImage = -1;
        emit sequenceEnd();
    }
    update();
}
