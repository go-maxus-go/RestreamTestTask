#include "SlideCtrl.h"

#include <QImage>

#include "Obs/Holder.h"
#include "Obs/Observer.h"
#include "Model/Slide.h"


namespace Logic
{

SlideCtrl::SlideCtrl()
    : m_obsHolder(std::make_unique<Obs::Holder<Model::Slide>>())
{}

SlideCtrl::~SlideCtrl() = default;

bool SlideCtrl::addSlide(QRect rect, QString path)
{
    auto image = std::make_shared<QImage>();
    if (!image->load(path))
        return false;

    auto slide = std::make_shared<Model::Slide>(
        std::move(rect),
        std::move(path),
        std::move(image),
        Model::Slide::State::Ready);
    m_slides.push_back(slide);
    m_obsHolder->notifyCreated(slide);

    return true;
}

void SlideCtrl::attach(ObsPtr obs)
{
    m_obsHolder->attach(obs);
}
void SlideCtrl::detach(ObsPtr obs)
{
    m_obsHolder->detach(obs);
}

} // namespace Logic
