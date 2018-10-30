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
    Observer(SlideshowCtrl & ctrl) : ctrl{ctrl}
    {}

protected:
    void created(Model::SlidePtrC object) override
    {
        ctrl.m_slideshow->slides.push_back(object);
        ctrl.m_obsHolder->notifyUpdated(ctrl.m_slideshow);
    }

private:
    SlideshowCtrl & ctrl;
};

SlideshowCtrl::SlideshowCtrl(std::shared_ptr<SlideCtrl> slideCtrl)
    : m_slideshow{std::make_shared<Model::Slideshow>()}
    , m_slideCtrl{std::move(slideCtrl)}
    , m_obsHolder{std::make_unique<Obs::Holder<Model::Slideshow>>()}
    , m_observer{std::make_shared<SlideshowCtrl::Observer>(*this)}
{
    m_slideCtrl->attach(m_observer);
    const auto imageFolder = QDir::currentPath() + QDir::separator() + "images" + QDir::separator();
    const auto files = QDir(imageFolder).entryList(); // I can add filter for images only here.
    for (const auto & file : files)
        m_slideCtrl->addSlide(QRect(), imageFolder + file);
}

SlideshowCtrl::~SlideshowCtrl()
{
    m_slideCtrl->detach(m_observer);
}

} // namespace Logic
