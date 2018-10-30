#include "SlideCtrl.h"

#include <QImage>

#include "Obs/Holder.h"
#include "Obs/Observer.h"
#include "Model/Slide.h"


namespace Logic
{

static int s_thumbnailSize = 90;

SlideCtrl::SlideCtrl()
    : m_obsHolder(std::make_unique<Obs::Holder<Model::Slide>>())
{}

SlideCtrl::~SlideCtrl() = default;

bool SlideCtrl::addSlide(QRect rect, QString path)
{
    auto image = std::make_shared<QImage>();
    if (!image->load(path))
        return false;
    *image = image->width() > image->height()
        ? image->scaledToWidth(s_thumbnailSize)
        : image->scaledToHeight(s_thumbnailSize);

    auto slide = std::make_shared<Model::Slide>(
        std::move(rect),
        std::move(path),
        std::move(image),
        Model::Slide::State::Ready);
    m_slides.push_back(slide);
    m_obsHolder->notifyCreated(slide);

    return true;
}

void SlideCtrl::attach(Obs::SlideObsPtr obs)
{
    m_obsHolder->attach(obs);
}
void SlideCtrl::detach(Obs::SlideObsPtr obs)
{
    m_obsHolder->detach(obs);
}

} // namespace Logic
