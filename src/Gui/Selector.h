#pragma once

#include <QWidget>

class QPoint;

class Selector : public QWidget
{
    Q_OBJECT
public:
    Selector(QWidget * parent = nullptr);

    void setPosition(const QPoint &);

protected:
    void paintEvent(QPaintEvent *);

private:
    const int m_width = 100; // hardcode =(
    const int m_height = 100;
    const int m_penWidth = 5;
};
