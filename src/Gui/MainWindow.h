#pragma once

#include <memory>

#include <QMainWindow>

#include "Logic/Fwd.h"


namespace Ui { class MainWindow; }
namespace Logic { class SlideshowCtrl; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Logic::SlideshowCtrlPtr slideshowCtrl, QWidget * parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow * ui;
    class Observer;
    std::shared_ptr<Observer> m_observer;
    Logic::SlideshowCtrlPtr m_slideshowCtrl;
};
