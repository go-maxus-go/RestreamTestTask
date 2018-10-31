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


class MainWindow::Observer : public Obs::SlideshowObs, public Obs::SlideObs
{
public:
    Observer(MainWindow & base) : m_base{base}
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
        m_slide2label[slide] = label;

        label->setGeometry(slide->rect);
        label->setAlignment(Qt::AlignCenter);
        updateImage(slide, label);
        updateSelector(slide);
    }
    void updated(Model::SlidePtrC slide) override
    {
        const auto & it = m_slide2label.find(slide);
        if (it == m_slide2label.end())
            return;
        auto & label = it->second;

        label->setGeometry(slide->rect);
        updateImage(slide, label);
        updateSelector(slide);
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
            {
                selector = new Selector(m_base.centralWidget());
            }
            m_slide2selector.first = slide;
            selector->setPosition(slide->rect.center());
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
    , m_observer{std::make_shared<Observer>(*this)}
    , m_slideshowCtrl{std::move(slideshowCtrl)}
    , m_slideCtrl{std::move(slideCtrl)}
{
    m_ui->setupUi(this);

    m_slideshowCtrl->attach(m_observer);
    m_slideCtrl->attach(m_observer);

    m_slideshowCtrl->loadSlideshow();
}

MainWindow::~MainWindow()
{
    m_slideshowCtrl->detach(m_observer);
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    switch (e->key())
    {
    case Qt::Key_Up:
        m_slideCtrl->selectTop();
        break;
    case Qt::Key_Down:
        m_slideCtrl->selectBottom();
        break;
    case Qt::Key_Left:
        m_slideCtrl->selectLeft();
        break;
    case Qt::Key_Right:
        m_slideCtrl->selectRight();
        break;
    }
}
