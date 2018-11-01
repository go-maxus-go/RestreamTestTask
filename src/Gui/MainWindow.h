#pragma once

#include <memory>

#include <QMainWindow>

#include "Logic/Fwd.h"


namespace Ui { class MainWindow; }

namespace Gui
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
            Logic::SlideshowCtrlPtr slideshowCtrl,
            Logic::SlideCtrlPtr slideCtrl,
            QWidget * parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    Logic::SlideshowCtrlPtr m_slideshowCtrl;
    Logic::SlideCtrlPtr m_slideCtrl;
    class Impl;
    std::shared_ptr<Impl> m_impl;
};

} // namespace Gui
