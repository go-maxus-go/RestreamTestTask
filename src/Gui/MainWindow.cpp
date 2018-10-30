#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLabel>
#include <QPixmap>

#include "Obs/Observer.h"
#include "Logic/SlideshowCtrl.h"
#include "Model/Slideshow.h"


class MainWindow::Observer : public Obs::SlideshowObs
{
public:
    Observer(MainWindow & base) : m_base{base}
    {}

    void updated(Model::SlideshowPtrC slideshow) override
    {
        for (const auto & slide : slideshow->slides)
        {
            const bool isReady = slide->state == Model::Slide::State::Ready;
            const bool isShown = m_slide2label.find(slide) != m_slide2label.end();
            if (isReady && !isShown)
            {
                auto label = new QLabel(m_base.centralWidget());
                label->setPixmap(QPixmap::fromImage(*slide->image));
                label->setGeometry(slide->rect);
                m_slide2label[slide] = label;
            }
        }
    }

private:
    MainWindow & m_base;
    std::map<Model::SlidePtrC, QLabel*> m_slide2label;
};

MainWindow::MainWindow(Logic::SlideshowCtrlPtr slideshowCtrl, QWidget * parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , m_observer{std::make_shared<Observer>(*this)}
    , m_slideshowCtrl{std::move(slideshowCtrl)}
{
    ui->setupUi(this);
    m_slideshowCtrl->attach(m_observer);
    m_slideshowCtrl->loadSlideshow();
}

MainWindow::~MainWindow()
{
    delete ui;
}
