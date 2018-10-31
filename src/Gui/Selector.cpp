#include "Selector.h"

#include <QPen>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QPaintEvent>


Selector::Selector(QWidget * parent) : QWidget{parent}
{
    setGeometry(0, 0, m_width + 2 * m_penWidth, m_height + 2 * m_penWidth);
}

void Selector::setPosition(const QPoint & pos)
{
    QRect rect = this->rect();
    rect.moveCenter(pos);
    setGeometry(rect);
}

void Selector::paintEvent(QPaintEvent * e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, m_penWidth));
    painter.drawRect(m_penWidth, m_penWidth,
                     rect().width() - 2 * m_penWidth,
                     rect().height() - 2 * m_penWidth);
}
