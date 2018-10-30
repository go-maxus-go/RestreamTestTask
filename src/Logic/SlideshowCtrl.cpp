#include "SlideshowCtrl.h"

#include <QDir>

#include "Obs/Holder.h"
#include "Obs/Observer.h"

#include "Model/Slide.h"
#include "Model/Slideshow.h"

#include "SlideCtrl.h"

namespace Logic
{

class SlideshowCtrl::Observer : public Obs::Observer<Model::Slide>
{
public:
    Observer(SlideshowCtrl & ctrl) : m_ctrl{ctrl}
    {}

protected:
    void created(Model::SlidePtrC object) override
    {
        m_ctrl.m_slideshow->slides.push_back(object);
        m_ctrl.m_obsHolder->notifyUpdated(m_ctrl.m_slideshow);
    }

private:
    SlideshowCtrl & m_ctrl;
};

SlideshowCtrl::SlideshowCtrl(SlideCtrlPtr slideCtrl)
    : m_slideshow{std::make_shared<Model::Slideshow>()}
    , m_slideCtrl{std::move(slideCtrl)}
    , m_obsHolder{std::make_unique<Obs::Holder<Model::Slideshow>>()}
    , m_observer{std::make_shared<SlideshowCtrl::Observer>(*this)}
{
    m_slideCtrl->attach(m_observer);
}

SlideshowCtrl::~SlideshowCtrl()
{
    m_slideCtrl->detach(m_observer);
}

void SlideshowCtrl::attach(Obs::SlideshowObsPtr obs)
{
    m_obsHolder->attach(obs);
}
void SlideshowCtrl::detach(Obs::SlideshowObsPtr obs)
{
    m_obsHolder->detach(obs);
}

void SlideshowCtrl::loadSlideshow()
{
    const auto imageFolder = QDir::currentPath() + QDir::separator() + "images" + QDir::separator();
    const auto files = QDir(imageFolder).entryList(
        QDir::Filter::Files | QDir::Filter::NoDotAndDotDot);

    QRect rect(0, 0, 100, 100);
    for (const auto & file : files) {
        m_slideCtrl->addSlide(rect, imageFolder + file);
        rect.moveCenter(rect.center() + QPoint(rect.width(), 0));
    }
}

} // namespace Logic
