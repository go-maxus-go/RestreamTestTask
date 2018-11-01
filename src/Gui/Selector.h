#pragma once

#include <QWidget>


class QRect;
class QPaintEvent;

namespace Gui
{

class Selector : public QWidget
{
    Q_OBJECT
public:
    using QWidget::QWidget;

    void setItemRect(const QRect &);

protected:
    void paintEvent(QPaintEvent *);

private:
    const int m_margin = 10;
    const int m_penWidth = 5;
};

} // namespace Gui
