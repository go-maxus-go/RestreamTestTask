#include "Selector.h"

#include <QPen>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>


namespace Gui
{

void Selector::setItemRect(const QRect & rect)
{
    setGeometry(rect.adjusted(-m_margin, -m_margin, m_margin, m_margin));
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

} //namespace Gui
