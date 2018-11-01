#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>

#include "Obs/Observer.h"
#include "Logic/SlideCtrl.h"
#include "Logic/SlideshowCtrl.h"
#include "Model/Slideshow.h"

#include "Selector.h"


namespace Gui
{

class MainWindow::Impl : public Obs::SlideshowObs, public Obs::SlideObs
{
public:
    Impl(MainWindow & base) : m_base{base}
    {
        m_slide2selector.second = nullptr;
    }

    void created(Model::SlideshowPtrC slideshow) override
    {
        m_base.setGeometry(0, 0, slideshow->width, slideshow->height);
    }

    void created(Model::SlidePtrC slide) override
    {
        auto label = new QLabel(m_base.centralWidget());
        label->setAlignment(Qt::AlignCenter);
        m_slide2label[slide] = label;
        updateSlide(slide);
    }
    void updated(Model::SlidePtrC slide) override
    {
        updateSlide(slide);
    }

    void updateSlide(Model::SlidePtrC slide)
    {
        updateSelector(slide);
        auto it = m_slide2label.find(slide);
        if (it == m_slide2label.end())
            return;
        auto label = it->second;
        label->setGeometry(slide->rect);
        updateImage(slide, label);
    }
    void updateImage(Model::SlidePtrC slide, QLabel * label)
    {
        const bool isReady = slide->state == Model::Slide::State::Ready;
        if (label->picture() == nullptr && isReady)
        {
            label->setPixmap(QPixmap::fromImage(*slide->image));
            label->setVisible(true);
        }
    }
    void updateSelector(Model::SlidePtrC slide)
    {
        if (slide->isSelected && slide != m_slide2selector.first)
        {
            auto & selector = m_slide2selector.second;
            if (selector == nullptr)
                selector = new Selector(m_base.centralWidget());

            m_slide2selector.first = slide;
            selector->setItemRect(slide->rect);
            selector->setVisible(true);
            selector->raise();
        }
        else if (!slide->isSelected && slide == m_slide2selector.first)
        {
            m_slide2selector.first.reset();
            if (auto selector = m_slide2selector.second)
                selector->setVisible(false);
        }
    }

private:
    MainWindow & m_base;
    std::map<Model::SlidePtrC, QLabel*> m_slide2label;
    std::pair<Model::SlidePtrC, Selector*> m_slide2selector;
};

MainWindow::MainWindow(
    Logic::SlideshowCtrlPtr slideshowCtrl,
    Logic::SlideCtrlPtr slideCtrl,
    QWidget * parent)
    : QMainWindow{parent}
    , m_ui{std::make_unique<Ui::MainWindow>()}
    , m_slideshowCtrl{std::move(slideshowCtrl)}
    , m_slideCtrl{std::move(slideCtrl)}
    , m_impl{std::make_shared<Impl>(*this)}
{
    m_ui->setupUi(this);

    m_slideshowCtrl->attach(m_impl);
    m_slideCtrl->attach(m_impl);

    m_slideshowCtrl->loadSlideshow();
}

MainWindow::~MainWindow()
{
    m_slideCtrl->attach(m_impl);
    m_slideshowCtrl->detach(m_impl);
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    Logic::SlideCtrl::Direction direction;
    switch (e->key())
    {
    case Qt::Key_Up:
        direction = Logic::SlideCtrl::Direction::Top;
        break;
    case Qt::Key_Down:
        direction = Logic::SlideCtrl::Direction::Bottom;
        break;
    case Qt::Key_Left:
        direction = Logic::SlideCtrl::Direction::Left;
        break;
    case Qt::Key_Right:
        direction = Logic::SlideCtrl::Direction::Right;
        break;
    default:
        return;
    }
    m_slideCtrl->moveSelection(direction);
}

} // namespace Gui
